#include <iostream>
#include <signal.h>
#include "Application.h"

using namespace std;

void sigint(int sig) {
    static int sigcount = 5;
    if (--sigcount <= 0) {
        raise(SIGTERM);
    }
    cout << "Trying to stop. (" << sigcount << " till SIGTERM)" << endl;
}

int main(int argc, const char** argv) {
    signal(SIGINT, sigint);
    Application app;
    while(app.run());
}
