#ifndef SEMICOLON_H
#define SEMICOLON_H

#include "Command.h"

class Semicolon: public Command {
    public:
    Semicolon(string command, Command* prev);
    void runCommand();
    void RedirectionAndPiping(char* args[], bool &commandFinished);
    string getType();
};


#endif