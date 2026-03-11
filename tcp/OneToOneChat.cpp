#include <arpa/inet.h> // inet_pton(), inet_ntoa()
#include <iostream>
#include <netinet/in.h> // sockaddr_in (IP + port structure)
#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <unistd.h>     // read(), write(), close()
using namespace std;
