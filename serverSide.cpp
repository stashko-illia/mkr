#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

#include <Server.hpp>

int main() {
    Server myServer = Server(5555);
    myServer.run();
    return 0;
}