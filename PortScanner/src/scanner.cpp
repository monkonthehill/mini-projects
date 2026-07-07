#include "../include/scanner.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <system_error>

void make_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to get file flags");
    }

    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to set non-blocking flag");
    }
}

PortState try_tcp_connect(const struct sockaddr* target_addr, socklen_t addrlen, int port)
{
    struct sockaddr_storage addr_copy;
    std::memcpy(&addr_copy, target_addr, addrlen);

    if (addr_copy.ss_family == AF_INET)
    {
        struct sockaddr_in* ipv4 = (struct sockaddr_in*) &addr_copy;
        ipv4->sin_port           = htons(port);
    }
    else if (addr_copy.ss_family == AF_INET6)
    {
        struct sockaddr_in6* ipv6 = (struct sockaddr_in6*) &addr_copy;
        ipv6->sin6_port           = htons(port);
    }

    int sockfd = socket(addr_copy.ss_family, SOCK_STREAM, 0);
    if (sockfd == -1)
        return PortState::ERROR;

    try
    {
        make_nonblocking(sockfd);
    }
    catch (...)
    {
        close(sockfd);
        return PortState::ERROR;
    }

    int rc         = connect(sockfd, (struct sockaddr*) &addr_copy, addrlen);
    bool connected = false;

    if (rc == 0)
    {
        connected = true;
    }
    else if (rc == -1 && errno == EINPROGRESS)
    {
        struct pollfd pfd;
        pfd.fd     = sockfd;
        pfd.events = POLLOUT;

        int timeout_ms = 50; 
        int poll_rc    = poll(&pfd, 1, timeout_ms);

        if (poll_rc > 0 && (pfd.revents & POLLOUT))
        {
            int socket_error = 0;
            socklen_t len    = sizeof(socket_error);
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &socket_error, &len) == 0)
            {
                if (socket_error == 0)
                {
                    connected = true;
                }
            }
        }
    }

    // Consolidated banner grabbing logic for all successful connections
    if (connected)
    {
        struct pollfd read_pfd;
        read_pfd.fd     = sockfd;
        read_pfd.events = POLLIN;

        // 1. Try passive reading first (for SSH, FTP, etc.)
        int read_rc = poll(&read_pfd, 1, 150); 
        bool banner_grabbed = false;

        if (read_rc > 0 && (read_pfd.revents & POLLIN))
        {
            char buffer[1024] = {0};
            int byte_read = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (byte_read > 0)
            {
                std::lock_guard<std::mutex> lock(result_mutex);
                std::cout << "[Port " << port << "] Banner Received (Passive): " << buffer << std::endl;
                banner_grabbed = true;
            }
        }

        // 2. If passive reading failed, fire an active probe payload (for HTTP, CUPS, etc.)
        if (!banner_grabbed)
        {
            // A generic, benign probe string that forces most TCP/HTTP engines to reply with an error/banner
            const char* generic_probe = "GET / HTTP/1.0\r\n\r\n";
            send(sockfd, generic_probe, strlen(generic_probe), 0);

            // Wait again to see if the service responds to our probe
            read_rc = poll(&read_pfd, 1, 250); 
            if (read_rc > 0 && (read_pfd.revents & POLLIN))
            {
                char buffer[1024] = {0};
                int byte_read = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
                if (byte_read > 0)
                {
                    std::lock_guard<std::mutex> lock(result_mutex);
                    // Extract just the first line of the server header response for cleaner formatting
                    std::string initial_reply(buffer);
                    size_t first_newline = initial_reply.find("\r\n");
                    if (first_newline != std::string::npos) {
                        initial_reply = initial_reply.substr(0, first_newline);
                    }
                    std::cout << "[Port " << port << "] Banner Received (Active): " << initial_reply << std::endl;
                    banner_grabbed = true;
                }
            }
        }

        if (!banner_grabbed)
        {
            std::lock_guard<std::mutex> lock(result_mutex);
            std::cout << "[Port " << port << "] Connected, but service remained silent to probes.\n";
        }

        // Thread-safe update to your shared scan results
        std::lock_guard<std::mutex> lock(result_mutex);
        open_port.push_back(port);
        close(sockfd);
        return PortState::OPEN;
    }
    close(sockfd);
    return PortState::CLOSED;
}
