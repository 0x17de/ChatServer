#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command {
private:
    std::string command_;
    std::string paramline_;
public:
    Command(std::string command, std::string paramline);

    std::string getCommand();
    std::string getParamLine();
};

#endif // COMMAND_H
