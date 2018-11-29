#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Command {
    
    protected:
    string cmd; //command to be run
    bool succeeded; //whether or not it succeeded
    Command* prev; //pointer to previous command
    bool ran;
    int groupPos;
    
    public:
    Command();
    Command(string command, Command* prev); //constructor
    //~Command();
    virtual void runCommand() = 0;
    string getCommand();
    bool getSuccess();
    Command* getPrev();
    void setGroup(int num);
    int getGroup();
    bool getRan();
    void setCommand(string str);
    void setPrev(Command* prv);
    virtual string getType() = 0;

};


#endif