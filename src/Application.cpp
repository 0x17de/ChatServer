#include "Application.h"

#include <iostream>
#include <list>

#include "Server.h"

using namespace std;

Application::Application() :
    server(12321)
{

}

bool Application::run() {
    return server.run();
}
