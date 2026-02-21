
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <unistd.h>
int main() {
  /*Programs never access resources directly.
They only use numeric handles given by the kernel.*/
  int fd = open("chatpipe", O_WRONLY);
  // int fd is used to perform this open action and also store that task id ;
  std::string msg;
  while (true) {
    std::getline(std::cin, msg);
    write(fd, msg.c_str(), msg.size());
    /*
     write sends raw bytes into pipe

     fd            -> connection
     msg.c_str()   -> convert string to char*
     msg.size()    -> number of bytes

     reader program immediately wakes up and receives this
 */
    close(fd);
  }
}
