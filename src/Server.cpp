#include "Server.h"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN
#include <windows.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#endif
#include <string.h>
#include <iostream>

using namespace std;

Server::Server(int port) {
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sa;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    bind(s, (sockaddr*)&sa, sizeof(sa));
    listen(s, 3);

    FD_ZERO(&readFd);
    FD_SET(s, &readFd);

    countFd = s + 1;
}

Server::~Server() {
    #ifdef _WIN
    closesocket(s);
    #else
    close(s);
    #endif // _WIN
}

bool Server::run() {
    memcpy(&readFdCopy, &readFd, sizeof(fd_set));
    timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (select(countFd, &readFdCopy, 0, 0, &tv) > 0) {
        if (FD_ISSET(s, &readFd)) {
            cout << "- New client" << endl;
        }
    }

    return true;
}
