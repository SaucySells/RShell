#ifndef NONE_H
#define NONE_H

#include "Command.h"

class None: public Command {
    public:
    None(string command, Command* prev);
    void runCommand();
    void RedirectionAndPiping(char* args[], bool &commandFinished);
    string getType();
};


#endif