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

    int id_;
    std::string name_;
public:
    Client(int fd);
    virtual ~Client();

    int getFd();
    bool recv();
    Command* getCommand();
    void send(std::string cmd, std::string data);
    void send(std::string data);

    int getId();
    std::string getName();
    void setName(std::string newname);
};

#endif // CLIENT_H
