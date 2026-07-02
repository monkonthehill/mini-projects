#include "../src/scanner.cpp" 

int main()
{
    std::string host;  
    char look_domain  = 'n';
    char look_port    = 'n';
    int port          = 0;
    int starting_port = 0;
    int ending_port   = 0;
    
    PortState result = PortState::CLOSED; 

    std::cout << "Do you want to look up a domain? (y/n): ";
    std::cin >> look_domain;

    if (look_domain == 'y' || look_domain == 'Y')
    {
        std::cout << "Enter the domain name (e.g., google.com): ";
        std::cin >> host;  
    }
    else
    {
        host = "127.0.0.1";
    }

    std::cout << "Do you want to look up for multiple ports? (y/n): ";
    std::cin >> look_port;

    struct addrinfo hints, *res = nullptr;
    std::memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(host.c_str(), nullptr, &hints, &res);
    if (status != 0) {
        std::cerr << "DNS Resolution Failed for " << host << ": " << gai_strerror(status) << "\n";
        return 1;
    }
    
    auto start_time = std::chrono::high_resolution_clock::now();

    if (look_port == 'y' || look_port == 'Y')
    {
        std::cout << "Enter the starting port number: ";
        std::cin >> starting_port;  
        std::cout << "Enter the ending port number: ";
        std::cin >> ending_port;
        std::cout << "Local scan is running !!! \n";
        std::cout << "Scanning from port " << starting_port << " to " << ending_port << "...\n";
        
        start_time = std::chrono::high_resolution_clock::now();

        const size_t MAX_CONCURRENT_THREADS = std::thread::hardware_concurrency() * 4; 
        std::vector<std::thread> threads;

        for (int ports = starting_port; ports <= ending_port; ++ports)
        {
            threads.push_back(std::thread(try_tcp_connect, res->ai_addr, res->ai_addrlen, ports));

            if (threads.size() >= MAX_CONCURRENT_THREADS || ports == ending_port)
            {
                for (auto& th : threads)
                {
                    if (th.joinable()) th.join();
                }
                threads.clear(); 
            }
        }
        
        auto stop_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

        std::cout << "\nScan Complete. Total open ports found: " << open_port.size() << "\n";
        std::cout << "Scan duration: " << duration.count() << " ms\n";
        
        freeaddrinfo(res);
        return 0; 
    }
    else
    {
        std::cout << "Enter the port number you want to check: ";
        std::cin >> port;
        std::cout << "Local scan is running !!! \n";

        start_time = std::chrono::high_resolution_clock::now();

        result = try_tcp_connect(res->ai_addr, res->ai_addrlen, port); 
    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

    if (result == PortState::OPEN)
    {
        std::cout << host << ":" << port << " is OPEN\n";
    }
    else if (result == PortState::CLOSED)
    {
        std::cout << host << ":" << port << " is CLOSED\n";
    }
    else
    {
        std::cout << "ERROR while scanning\n";
    }

    std::cout << "Scan duration: " << duration.count() << " ms\n";

    freeaddrinfo(res);
    return 0;
}
