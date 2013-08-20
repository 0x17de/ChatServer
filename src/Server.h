#ifndef SERVER_H
#define SERVER_H

#include <list>
#include <sys/types.h>

#include "Client.h"

class Server {
private:
    std::list<Client> clientList_;
    int s;
    fd_set readFd, readFdCopy;
    int countFd;
public:
    Server(int port);
    virtual ~Server();
    bool run();
    void stop();
};

#endif // SERVER_H
