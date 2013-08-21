#include "Client.h"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN
#include <windows.h>
#define MSG_DONTWAIT 0
#define MSG_NOSIGNAL 0
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

#include <iostream>

using namespace std;

Client::Client(int fd) :
    streamlength_(0)
{
    fd_ = fd;
    static int id = 0;
    id_ = id; ++id;
    stringstream ss;
    ss << "Guest_" << id_;
    name_ = ss.str();
    cout << "New client Id(" << id_ << ") Name: " << name_ << endl;

    stringstream ss2;
    ss2 << id_ << ":" << name_;
    send("HELLO", ss2.str());
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

int Client::getId() {
    return id_;
}

string Client::getName() {
    return name_;
}

bool Client::recv() {
    char buffer[1024];
    while(true) {
        int res = ::recv(fd_, buffer, 1023, MSG_DONTWAIT);
        if (res == 0) {
            return false;
        } else if (res < 0) {
            break;
        } else {
            streamlength_ += res;
            if (streamlength_ > 5000) {
                return false;
            }
            buffer[res] = 0;
            ss_ << buffer;
        }
    };
    cout << "DATA: " << ss_.str() << endl;
    return true;
}

Command* Client::getCommand() {
    string s = ss_.str();
    string command, paramline;
    int start = -1, n = -1;
    bool escape = false;
    bool complete = false;
    bool trim = true;
    for(char c : s) {
        ++n;
        if (trim) {
            if (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\0')
                continue;
            start = n;
            trim = false;
        }
        if (escape == true) {
            escape = false;
            continue;
        }
        if (c == '\\') {
            escape = true;
            continue;
        } else if (command == "" && (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\0')) {
            command = s.substr(start, n - start);
            trim = true;
        } else if (c == ';') {
            paramline = s.substr(start, n - start);
            complete = true;
            if (command == "") {
                ss_.str("");
                ss_.clear();
                return new Command("FAIL", "");
            }
            break;
        }
    }

    if (!complete || command == "")
        return nullptr;

    streamlength_ -= n;
    ss_.str("");
    ss_.clear();
    ss_ << s.substr(n + 1, s.length() - n);

    return new Command(command, paramline);
}

void Client::send(string data) {
    ::send(fd_, data.c_str(), data.length(), MSG_NOSIGNAL);
}

void Client::send(string cmd, string data) {
    stringstream ss;
    ss << cmd;
    ss << " ";
    ss << data;
    ss << ";\n";
    string s = ss.str();
    ::send(fd_, s.c_str(), s.length(), MSG_NOSIGNAL);
}

void Client::setName(string newname) {
    name_ = newname;
}

