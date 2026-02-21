#include <fcntl.h>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

const int b_size = 1024;

void recieve_string(int ls) {
  char buffer[b_size];

  while (true) {
    int bytes = read(ls, buffer, b_size - 1);

    if (bytes > 0) {
      buffer[bytes] = '\0';
      std::cout << "\nFriend: " << buffer << "\nYou: " << std::flush;
    }
  }
}

void send_string(int fd) {
  std::string msg;

  while (true) {
    std::cout << "You: ";
    std::getline(std::cin, msg);
    write(fd, msg.c_str(), msg.size());
  }
}

int main() {
  int ls = open("recieving", O_RDONLY);
  int fd = open("sending", O_WRONLY);

  std::thread t1(recieve_string, ls);
  std::thread t2(send_string, fd);

  t1.join();
  t2.join();
}
