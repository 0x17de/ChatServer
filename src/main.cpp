#include <iostream>
#include <signal.h>
#include "Application.h"

using namespace std;

Application app;

void sigint(int sig) {
    static int sigcount = 5;
    if (--sigcount <= 0) {
        raise(SIGTERM);
    }
    cout << endl << "Trying to stop. (" << sigcount << " till SIGTERM)" << endl;
    app.stop();
}

int main(int argc, const char** argv) {
    signal(SIGINT, sigint);
    while(app.run());
}
