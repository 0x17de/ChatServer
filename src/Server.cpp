#include "Server.h"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN
#include <windows.h>
#define sleep(x) Sleep(1000*x)
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
#include <algorithm>
#include <memory>

using namespace std;

#ifdef _WIN
static int sockcount = 0;

void unblockSocket(int fd) {
	u_long mode = 1;
	ioctlsocket(fd, FIONBIO, &mode); 
}
#endif

Server::Server(int port) {
	#ifdef _WIN
	if (sockcount == 0) {
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2,1), &wsaData);
		++sockcount;
	}
	#endif

    s_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    #ifdef _WIN
    unblockSocket(s_);
    #endif
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
    cout << "Server socket closed." << endl;

#ifdef _WIN
	--sockcount;
	if (sockcount == 0) {
		WSACleanup();
	}
#endif
}

Client* Server::acceptClient() {
    int cFd = accept(s_, 0, 0);
    #ifdef _WIN
    unblockSocket(cFd);
    #endif
    FD_SET(cFd, &readFd_);
    countFd_ = max(countFd_, cFd + 1);
    Client* c = new Client(cFd);
    unique_ptr<Client> uc;
    uc.reset(c);
    clientList_.push_back(move(uc));
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
            stringstream ss;
            ss << c->getName() << " was kicked (Invalid client action).";
            broadcast("MSG", ss.str());
            return false;
        } else if (cmdstr == "NEWNAME") {
            string newname = cmd->getParamLine();
            string lowernewname = newname;
            transform(begin(lowernewname), end(lowernewname), begin(lowernewname), ::tolower);
            bool found = false;
            for(unique_ptr<Client>& subc : clientList_) {
                string subcname = subc->getName();
                transform(begin(subcname), end(subcname), begin(subcname), ::tolower);
                cout << "COMPARE: " << subcname << ":" << lowernewname << endl;
                if (subcname == lowernewname) { // TODO check inside DB with password
                    found = true; break;
                }
            }
            if (found) {
                c->send("NAMEFAIL", newname);
            } else {
                c->send("NAMEOK", newname);
                c->setName(newname);
                stringstream ss;
                ss << "NAMECHANGE:" << c->getId();
                broadcast(ss.str(), newname);
            }
        } else if (cmdstr == "PING") {
            c->send("PONG", "");
        } else if (cmdstr == "MSG") {
            stringstream ss;
            ss << "MSG:" << c->getId();
            broadcast(ss.str(), cmd->getParamLine());
        }
    }
    return true;
}

bool Server::run() {
    if (select() > 0) {
        if (isset(s_)) {
            Client* c = acceptClient();
            cout << "- New client: " << c->getName() << endl;
            stringstream ss;
            ss << c->getId() << ":" << c->getName();
            broadcast("JOIN", ss.str());
            stringstream suserlist;
            for(unique_ptr<Client>& subc : clientList_) {
                suserlist << subc->getId() << ":" << subc->getName() << " ";
            }
            c->send("USERLIST", suserlist.str());
        }
        auto i = begin(clientList_);
        while(i != end(clientList_)) {
            unique_ptr<Client>& c = *i;
            if (isset(c)) {
                if (!c->recv() || !processCommands(c)) {
                    int cId = c->getId();
                    removeClient(i);
                    broadcast("LEAVE", to_string(cId));
                    continue;
                }
            }
            ++i;
        }
    }

    return true;
}
