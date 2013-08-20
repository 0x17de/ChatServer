#ifndef SERVER_H
#define SERVER_H

#include <list>
#include <memory>
#include <sys/types.h>

#include "Client.h"

class Server {
private:
    std::list<std::unique_ptr<Client> > clientList_;
    int s_;
    fd_set readFd_, readFdCopy_;
    int countFd_;
public:
    Server(int port);
    virtual ~Server();

    bool run();
    void stop();
    int select();
    int isset(int fd);
    int isset(std::unique_ptr<Client>& c);

    void acceptClient();
    void removeClient(std::list<std::unique_ptr<Client> >::iterator& i);
};

#endif // SERVER_H
