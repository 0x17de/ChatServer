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
#include <sstream>
#include <string>
#include <memory>

using namespace std;

Server::Server(int port) {
    s_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sa;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);

    int bindTryCount = 25;
    cout << "Trying to bind..." << endl;
    while (::bind(s_, (sockaddr*)&sa, sizeof(sa)) != 0) {
        if (--bindTryCount < 0)
            throw 0;
        sleep(1);
        cout << ".";
        cout.flush();
    }
    cout << endl;
    listen(s_, 3);

    FD_ZERO(&readFd_);
    FD_SET(s_, &readFd_);

    countFd_ = s_ + 1;
}

Server::~Server() {
    #ifdef _WIN
    closesocket(s_);
    #else
    close(s_);
    #endif
}

unique_ptr<Client>& Server::acceptClient() {
    int cFd = accept(s_, 0, 0);
    FD_SET(cFd, &readFd_);
    countFd_ = max(countFd_, cFd + 1);
    unique_ptr<Client> c;
    c.reset(new Client(cFd));
    clientList_.push_back(move(c));
    return c;
}

int Server::select() {
    timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    memcpy(&readFdCopy_, &readFd_, sizeof(fd_set));
    return ::select(countFd_, &readFdCopy_, nullptr, nullptr, &tv);
}

int Server::isset(int fd) {
    return FD_ISSET(fd, &readFdCopy_);
}

int Server::isset(unique_ptr<Client>& c) {
    return isset(c->getFd());
}

void Server::removeClient(list<unique_ptr<Client> >::iterator& i) {
    unique_ptr<Client>& c = *i;
    FD_CLR(c->getFd(), &readFd_);
    i = clientList_.erase(i);
}

void Server::broadcast(string cmd, string data) {
    stringstream ss;
    ss << cmd;
    ss << " ";
    ss << data;
    ss << ";\n";
    string s = ss.str();
    for(unique_ptr<Client>& c : clientList_) {
        c->send(s);
    }
}

bool Server::processCommands(unique_ptr<Client>& c) {
    unique_ptr<Command> cmd;
    while (cmd.reset(c->getCommand()), cmd != nullptr) {
        string cmdstr = cmd->getCommand();
        cout << "COMMAND: " <<  cmdstr << endl;
        if (cmdstr == "FAIL") {
            return false;
        } else if (cmdstr == "MSG") {
            broadcast("MSG", cmd->getParamLine());
        }
    }
    return true;
}

bool Server::run() {
    if (select() > 0) {
        if (isset(s_)) {
            cout << "- New client" << endl;
            unique_ptr<Client>& c = acceptClient();
            broadcast("JOIN", "USER");
        }
        auto i = begin(clientList_);
        while(i != end(clientList_)) {
            unique_ptr<Client>& c = *i;
            if (isset(c)) {
                if (!c->recv() || !processCommands(c)) {
                    removeClient(i);
                    broadcast("LEAVE", "USER");
                    continue;
                }
            }
            ++i;
        }
    }

    return true;
}
