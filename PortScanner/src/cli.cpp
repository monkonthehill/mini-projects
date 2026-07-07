#include "../src/scanner.cpp"
#include <getopt.h>
#include <sstream>
#include <algorithm>

static void print_usage(const char* prog)
{
    std::cout << "Usage: " << prog << " -H <host> -p <ports> [-t threads]\n"
              << "  -H, --host <host>       Target host or IP (default: 127.0.0.1)\n"
              << "  -p, --ports <spec>      Port(s): single (80), range (20-100), list (22,80,443)\n"
              << "  -t, --threads <n>       Max concurrent threads (default: hw_concurrency * 4)\n"
              << "  -h, --help              Show this help message\n";
}

static bool parse_port_spec(const std::string& spec, std::vector<int>& ports)
{
    std::stringstream ss(spec);
    std::string token;

    while (std::getline(ss, token, ','))
    {
        size_t dash = token.find('-');
        if (dash != std::string::npos)
        {
            try
            {
                int start = std::stoi(token.substr(0, dash));
                int end   = std::stoi(token.substr(dash + 1));
                if (start > end || start < 1 || end > 65535)
                    return false;
                for (int p = start; p <= end; ++p)
                    ports.push_back(p);
            }
            catch (...)
            {
                return false;
            }
        }
        else
        {
            try
            {
                int p = std::stoi(token);
                if (p < 1 || p > 65535)
                    return false;
                ports.push_back(p);
            }
            catch (...)
            {
                return false;
            }
        }
    }
    return !ports.empty();
}

int main(int argc, char* argv[])
{
    std::string host = "127.0.0.1";
    std::string port_spec;
    size_t max_threads = std::thread::hardware_concurrency() * 4;
    if (max_threads == 0)
        max_threads = 8;

    static struct option long_opts[] = {
        {"host",    required_argument, nullptr, 'H'},
        {"ports",   required_argument, nullptr, 'p'},
        {"threads", required_argument, nullptr, 't'},
        {"help",    no_argument,       nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "H:p:t:h", long_opts, nullptr)) != -1)
    {
        switch (opt)
        {
            case 'H':
                host = optarg;
                break;
            case 'p':
                port_spec = optarg;
                break;
            case 't':
                max_threads = static_cast<size_t>(std::stoi(optarg));
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    if (port_spec.empty())
    {
        std::cerr << "Error: no ports specified. Use -p/--ports.\n";
        print_usage(argv[0]);
        return 1;
    }

    std::vector<int> ports;
    if (!parse_port_spec(port_spec, ports))
    {
        std::cerr << "Error: invalid port specification '" << port_spec << "'\n";
        return 1;
    }

    struct addrinfo hints, *res = nullptr;
    std::memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(host.c_str(), nullptr, &hints, &res);
    if (status != 0)
    {
        std::cerr << "DNS Resolution Failed for " << host << ": " << gai_strerror(status) << "\n";
        return 1;
    }

    std::cout << "Scanning " << host << " (" << ports.size() << " port"
              << (ports.size() > 1 ? "s" : "") << ")...\n";

    auto start_time = std::chrono::high_resolution_clock::now();

    if (ports.size() == 1)
    {
        PortState result = try_tcp_connect(res->ai_addr, res->ai_addrlen, ports[0]);
        auto stop_time = std::chrono::high_resolution_clock::now();
        auto duration  = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

        if (result == PortState::OPEN)
            std::cout << host << ":" << ports[0] << " is OPEN\n";
        else if (result == PortState::CLOSED)
            std::cout << host << ":" << ports[0] << " is CLOSED\n";
        else
            std::cout << "ERROR while scanning\n";

        std::cout << "Scan duration: " << duration.count() << " ms\n";
    }
    else
    {
        std::vector<std::thread> threads;
        for (size_t i = 0; i < ports.size(); ++i)
        {
            threads.push_back(std::thread(try_tcp_connect, res->ai_addr, res->ai_addrlen, ports[i]));
            if (threads.size() >= max_threads || i == ports.size() - 1)
            {
                for (auto& th : threads)
                    if (th.joinable()) th.join();
                threads.clear();
            }
        }

        auto stop_time = std::chrono::high_resolution_clock::now();
        auto duration  = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

        std::sort(open_port.begin(), open_port.end());
        std::cout << "\nScan Complete. Open ports (" << open_port.size() << "): ";
        for (size_t i = 0; i < open_port.size(); ++i)
            std::cout << open_port[i] << (i + 1 < open_port.size() ? ", " : "");
        std::cout << "\nScan duration: " << duration.count() << " ms\n";
    }

    freeaddrinfo(res);
    return 0;
}
