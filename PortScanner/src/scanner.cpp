#include "../include/scanner.hpp"

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
// Pass the pre-resolved address structures into the function
PortState try_tcp_connect(const struct sockaddr* target_addr, socklen_t addrlen, int port)
{
    // Clone the address structure so we can safely modify the port field
    struct sockaddr_storage addr_copy;
    std::memcpy(&addr_copy, target_addr, addrlen);

    // Inject the current port into the copied address structure
    if (addr_copy.ss_family == AF_INET) {
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)&addr_copy;
        ipv4->sin_port = htons(port);
    } else if (addr_copy.ss_family == AF_INET6) {
        struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)&addr_copy;
        ipv6->sin6_port = htons(port);
    }

    int sockfd = socket(addr_copy.ss_family, SOCK_STREAM, 0);
    if (sockfd == -1) return PortState::ERROR;

    try {
        make_nonblocking(sockfd);
    } catch (...) {
        close(sockfd);
        return PortState::ERROR;
    }

    // Attempt connection
    int rc = connect(sockfd, (struct sockaddr*)&addr_copy, addrlen);
    bool connected = false;

    if (rc == 0) {
        connected = true;
    } else if (rc == -1 && errno == EINPROGRESS) {
        struct pollfd pfd;
        pfd.fd = sockfd;
        pfd.events = POLLOUT;

        // On localhost, connections fail or succeed instantly. 
        // Dropping timeout to 10-50ms here prevents lingering delays.
        int timeout_ms = 50; 
        int poll_rc = poll(&pfd, 1, timeout_ms);

        if (poll_rc > 0) {
            int socket_error = 0;
            socklen_t len = sizeof(socket_error);
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &socket_error, &len) == 0) {
                if (socket_error == 0) {
                    connected = true;
                }
            }
        }
    }

    close(sockfd);

    if (connected) {
        std::lock_guard<std::mutex> lock(result_mutex);
        open_port.push_back(port);
        return PortState::OPEN;
    }

    return PortState::CLOSED;
}
