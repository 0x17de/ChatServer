#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <sstream>

#include "Command.h"

class Client {
private:
    int fd_;
    std::stringstream ss_;
    int streamlength_;
public:
    Client(int fd);
    virtual ~Client();

    int getFd();
    bool recv();
    Command* getCommand();
    void send(std::string cmd, std::string data);
    void send(std::string data);
};

#endif // CLIENT_H
