#include "Command.h"

#include <string>

using namespace std;

Command::Command(string command, string paramline) :
    command_(command),
    paramline_(paramline)
{ }

string Command::getCommand() {
    return command_;
}

string Command::getParamLine() {
    return paramline_;
}

