#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>

#include "Server.h"

class Application {
private:
    Server server;
public:
    Application();
    bool run();
};

#endif // APPLICATION_H
