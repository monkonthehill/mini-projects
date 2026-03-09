/*
    The ai_addr field inside struct addrinfo is a generic pointer (sockaddr*).
    However, the actual address structure can be different depending on the
    IP version returned by getaddrinfo().

    If the address family is AF_INET → it means the address is IPv4.
    In this case, we cast the generic sockaddr pointer to sockaddr_in,
    which is the structure used for IPv4 addresses. The actual IPv4 address
    is stored inside the sin_addr field.

    If the address family is AF_INET6 → it means the address is IPv6.
    Here we cast the pointer to sockaddr_in6, which stores IPv6 addresses.
    The actual IPv6 address is inside the sin6_addr field.

    The extracted address pointer is stored in 'addr', so it can later be
    passed to inet_ntop() to convert the binary IP address into a readable
    string format.

    getaddrinfo() can return multiple results (IPv4 and/or IPv6), so this
    logic ensures we correctly handle both formats.
*/
#include <arpa/inet.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  std::string domain;
  std::cout << "Enter the domain you want to search the ip for: ";
  std::cin >> domain;
  struct addrinfo hints, *res, *p;
  char ipstr[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int status = getaddrinfo(domain.c_str(), NULL, &hints, &res);
  if (status != 0) {
    std::cerr << "getaddrinfo error " << gai_strerror(status) << std::endl;
    return 0;
  }
  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;
    std::string ipver;
    if (p->ai_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }
    if (p->ai_family == AF_INET6) {
      struct sockaddr_in *ipv6 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv6->sin_addr);
      ipver = "IPv6";
    }
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    std::cout << "  " << ipver << ": " << ipstr << std::endl;
  }
  freeaddrinfo(res);
  return 0;
}
