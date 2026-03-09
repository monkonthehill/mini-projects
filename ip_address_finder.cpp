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
