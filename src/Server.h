#ifndef SERVER_H
#define SERVER_H

#if defined(_WIN32) || defined(_WIN64)
#define _WIN
#include <windows.h>
#else
#include <sys/types.h>
#endif

#include <list>
#include <memory>

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

    bool processCommands(std::unique_ptr<Client>& c);

    Client* acceptClient();
    void removeClient(std::list<std::unique_ptr<Client> >::iterator& i);
    void broadcast(std::string cmd, std::string data);
};

#endif // SERVER_H
