#include <iostream>
#include <signal.h>
#include "Application.h"

using namespace std;

Application app;

void sigint(int sig) {
    static int sigcount = 5;
    if (--sigcount <= 0) {
        cout << "Using SIGTERM" << endl;
        raise(SIGTERM);
    }
    cout << endl << "Trying to stop (SIGINT). (" << sigcount << " till SIGTERM)" << endl;
    app.stop();
}

void sigterm(int sigterm) {
    static int sigcount = 2;
    if (--sigcount <= 0) {
        cout << "Using SIGKILL" << endl;
        raise(SIGKILL);
    }
    cout << endl << "Trying to stop (SIGTERM). (next time SIGKILL)" << endl;
    app.stop();
}

int main(int argc, const char** argv) {
    signal(SIGINT, sigint);
    signal(SIGTERM, sigterm);
    while(app.run());
}
