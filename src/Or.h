#ifndef OR_H
#define OR_H

#include "Command.h"

class Or: public Command {
    public:
    Or(string command, Command* prev);
    void runCommand();
    void RedirectionAndPiping(char* args[], bool &commandFinished);
    string getType();
};


#endif