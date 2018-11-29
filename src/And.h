#ifndef AND_H
#define AND_H

#include "Command.h"

class And: public Command {
    public:
    And(string command, Command* prev);
    void runCommand();
    void RedirectionAndPiping(char* args[], bool &commandFinished);
    string getType();
};


#endif