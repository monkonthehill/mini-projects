#include <fcntl.h>
#include <iostream>
#include <iterator>
#include <ostream>
#include <unistd.h>

int main() {
  char buffer[1024];
  int fd = open("chatpipe", O_RDWR);
  while (true) {
    int bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes > 0) {
      buffer[bytes] = '\0';
      std::cout << "User = " << buffer << std::endl
                << "the sizeof of message is = " << bytes;
    }
  }
}
