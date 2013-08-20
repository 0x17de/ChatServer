#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>

#include "Server.h"

class Application {
private:
    Server server_;
    bool running_;
public:
    Application();
    virtual ~Application();

    bool run();
    void stop();
};

#endif // APPLICATION_H
