#include "Server.h"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN
#include <windows.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

Server::Server(int port) {
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sa;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    bind(s, (sockaddr*)&sa, sizeof(sa));
    listen(s, 3);
}

Server::~Server() {
    #ifdef _WIN
    closesocket(s);
    #else
    close(s);
    #endif // _WIN
}

bool Server::run() {
    return true;
}
