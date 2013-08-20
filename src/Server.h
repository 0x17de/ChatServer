#ifndef SERVER_H
#define SERVER_H

#include <list>

#include "Client.h"

class Server {
private:
    std::list<Client> clientList_;
    int s;
public:
    Server(int port);
    virtual ~Server();
    bool run();
};

#endif // SERVER_H
