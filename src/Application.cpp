#include "Application.h"

#include <iostream>
#include <list>

#include "Server.h"

using namespace std;

Application::Application() :
    server(12321),
    running(true)
{
    cout << "Server is starting" << endl;
}

Application::~Application() {
    cout << "Application is shutting down" << endl;
}

void Application::stop() {
    cout << "Server is stopping" << endl;
    running = false;
}

bool Application::run() {
    return running && server.run();
}
