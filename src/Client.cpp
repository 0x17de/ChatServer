#include "Client.h"


#ifdef _WIN
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>

using namespace std;

Client::Client(int fd) {
    cout << "New client" << endl;
    fd_ = fd;
}

Client::~Client() {
    cout << "Delete client" << endl;
    #ifdef _WIN
    closesocket(fd_);
    #else
    close(fd_);
    #endif
}

int Client::getFd() {
    return fd_;
}
