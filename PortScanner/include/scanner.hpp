#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>   // C++ wrapper for fcntl.h
#include <poll.h>
#include <cerrno>   
#include <netinet/in.h>  // sockaddr_in (IP + port structure)
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <thread>
#include <mutex>
#include <system_error> 
#include <iostream>
#include <string>

#define MYPORT "3490"
std::mutex result_mutex;
std::vector<int> open_port;

enum class PortState
{
    OPEN,
    CLOSED,
    TIMEOUT,
    ERROR
};

