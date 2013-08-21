#include "Application.h"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN
#endif

#include <iostream>
#include <list>

#include "Server.h"

using namespace std;

Application::Application() :
    server_(12321),
    running_(true)
{
    cout << "Server is starting" << endl;
}

Application::~Application() {
    cout << "Application is shutting down" << endl;
}

void Application::stop() {
    cout << "Server is stopping" << endl;
    running_ = false;
}

bool Application::run() {
    return running_ && server_.run();
}
