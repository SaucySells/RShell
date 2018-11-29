#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdio.h>

#include "Command.h"
#include "commandGroup.h"
#include "Semicolon.h"
#include "None.h"
#include "Or.h"
#include "And.h"

using namespace std;

void parseInput(char input[], vector<Command*> &commandList) {
    char* pch;
    int i = -1;
    string connector = "none";
    pch = strtok(input, " ");
    string cmd(pch); 
    while (pch != NULL) {
        pch = strtok(NULL, " ");
        if (pch != NULL) {
            if (cmd != "") {
                cmd += " ";
            }
            cmd += string(pch);
        }
        if (cmd.at(cmd.length()-1) == ';') {
            if (connector == "none") {
                cmd.erase(cmd.length()-1, cmd.length()-1);
                commandList.push_back(new None(cmd, NULL));
                ++i;
            }
            if (connector == "semi") {
                cmd.erase(cmd.length()-1, cmd.length()-1);
                commandList.push_back(new Semicolon(cmd, commandList.at(i)));
                ++i;
            }
            if (connector == "and") {
                cmd.erase(cmd.length()-1, cmd.length()-1);
                commandList.push_back(new And(cmd, commandList.at(i)));
                ++i;
            }
            if (connector == "or") {
                cmd.erase(cmd.length()-1, cmd.length()-1);
                commandList.push_back(new Or(cmd, commandList.at(i)));
                ++i;
            }
            connector = "semi";
            cmd = "";
        }
        if (pch == NULL) {
            if (connector == "none") {
                commandList.push_back(new None(cmd, NULL));
                ++i;
            }
            if (connector == "semi") {
                commandList.push_back(new Semicolon(cmd, commandList.at(i)));
                ++i;
            }
            if (connector == "and") {
                commandList.push_back(new And(cmd, commandList.at(i)));
                ++i;
            }
            if (connector == "or") {
                commandList.push_back(new Or(cmd, commandList.at(i)));
                ++i;
            }
            //cmd = "";
        }
        else if (string(pch) == "||") {
            if (connector == "none") {
                cmd.erase(cmd.length()-3, cmd.length()-1);
                commandList.push_back(new None(cmd, NULL));
                ++i;
            }
            if (connector == "semi") {
                cmd.erase(cmd.length()-3, cmd.length()-1);
                commandList.push_back(new Semicolon(cmd, commandList.at(i)));
                ++i;
            }
            if (connector == "and") {
                cmd.erase(cmd.length()-3, cmd.length()-1);
                commandList.push_back(new And(cmd, commandList.at(i)));
                ++i;
            }
            if (connector == "or") {
                cmd.erase(cmd.length()-3, cmd.length()-1);
                commandList.push_back(new Or(cmd, commandList.at(i)));
                ++i;
            }
            connector = "or";
            cmd = "";
        }
        else if (string(pch) == "&&") {
            if (connector == "none") {
                cmd.erase(cmd.length()-3, cmd.length()-1);
                commandList.push_back(new None(cmd, NULL));
                ++i;
            }
            if (connector == "semi") {
                cmd.erase(cmd.length()-3, cmd.length()-1);
                commandList.push_back(new Semicolon(cmd, commandList.at(i)));
                ++i;
            }
            if (connector == "and") {
                cmd.erase(cmd.length()-3, cmd.length()-1);
                commandList.push_back(new And(cmd, commandList.at(i)));
                ++i;
            }
            if (connector == "or") {
                cmd.erase(cmd.length()-3, cmd.length()-1);
                commandList.push_back(new Or(cmd, commandList.at(i)));
                ++i;
            }
            connector = "and";
            cmd = "";
        }
    }
    int parenthesisCheck = 0;
    int currOutermost = 0;
    vector<commandGroup*> commandGroupList;
    for (unsigned i = 0; i < commandList.size(); ++i) {
        Command* temp = commandList.at(i);
        if (temp->getCommand().at(0) == '(') {
            if (parenthesisCheck > 0) { //nested parenthesis
                commandGroupList.push_back(new commandGroup(temp->getPrev(), temp->getType()));
                commandGroupList.back()->setGroup((commandGroupList.size()-parenthesisCheck)-1);
                temp->setCommand(temp->getCommand().erase(0,1));
                temp->setPrev(NULL);
                temp->setGroup((commandGroupList.size()-1));
                --i;
                parenthesisCheck += 1;
            }
            else if (parenthesisCheck == 0) {
                commandGroupList.push_back(new commandGroup(temp->getPrev(), temp->getType())); //creates commandGroup object, adds to vector
                temp->setCommand(temp->getCommand().erase(0,1)); //erases parenthesis
                temp->setPrev(NULL);
                currOutermost = commandGroupList.size()-1;
                temp->setGroup(commandGroupList.size()-1);
                --i;
                parenthesisCheck += 1;
            }
        }
        else if (temp->getCommand().at(temp->getCommand().size()-1) == ')' && parenthesisCheck != 0) {
            if (parenthesisCheck > 1) {
                temp->setGroup(commandGroupList.size()-1);
                temp->setCommand(temp->getCommand().erase(temp->getCommand().size()-1,1));
                --i;
                parenthesisCheck -= 1;
            }
            else if (parenthesisCheck == 1) {
                if (temp->getGroup() == -1) { 
                    temp->setGroup(currOutermost);
                }
                temp->setCommand(temp->getCommand().erase(temp->getCommand().size()-1,1));
                --i;
                parenthesisCheck -= 1;
            }
            else if (parenthesisCheck <= 0) {
                //error
                perror("unbalanced parenthesis");
                commandList.clear();
                return;
            }
        }
        else {
            //fixedList.push_back(temp);
            if (parenthesisCheck > 0) { //inside parenthesis
                temp->setGroup(commandGroupList.size()-1);
            }
        }
    }
    if (parenthesisCheck != 0) {
        perror("unbalanced parenthesis");
        commandList.clear();
        return;
        //error
    }
    for (unsigned i = 0; i < commandList.size(); ++i) {
        Command* temp = commandList.at(i);
        if (temp->getGroup() != -1) {
            commandGroupList.at(temp->getGroup())->addCommand(temp);
            commandList.erase(commandList.begin()+i);
            --i;
        }
    }
    for (unsigned i = 0; i < commandGroupList.size(); ++i) {
        commandGroup* temp = commandGroupList.at(i);
        if (temp->getGroup() != -1) {
            commandGroupList.at(temp->getGroup())->addCommand(temp);
            commandGroupList.erase(commandGroupList.begin() + i);
            --i;
        }
    }
    for (unsigned i = 0; i < commandGroupList.size(); ++i) {
        commandList.push_back(commandGroupList.at(i));
    }
    //cout << commandList.size() << endl;
    Command* lastRun = NULL;    
    for (unsigned i = 0; i < commandList.size(); ++i)  {
        for (unsigned j = i+1; j < commandList.size(); ++j) {
             if (commandList.at(i)->getPrev() != lastRun) {
                 Command* temp = commandList.at(i);
                 commandList.at(i) = commandList.at(j);
                 commandList.at(j) = temp;
             }
        }
        lastRun = commandList.at(i);
    }
    
    /*cout << commandGroupList.size() << endl;
    for (unsigned i = 0; i < commandGroupList.size(); ++i) {
        for (unsigned j = 0; j < commandGroupList.at(i)->numCommands(); ++j) {
            cout << commandGroupList.at(i)->ReturnCommand(j) << "; ";
        }
        cout << endl;
    }*/
}


int main() {
    vector<Command*> commandList; //vector to hold commands
    string str;
    int i = 0;
    while (1) {
        commandList.clear();
        cout << "$ ";
        getline(cin, str);
        size_t temp = str.find("#");
        if (temp != string::npos) { //removes comments
            str.erase(temp, str.length()-1);
        }
        if (str.length() != 0) {
            char* input = new char [str.length()+1]; 
            strcpy(input, str.c_str()); //converts string to c_string
            parseInput(input, commandList);
            for (unsigned i = 0; i < commandList.size(); ++i) {
                commandList.at(i)->runCommand();
                //cout << commandList.at(i)->getCommand() << endl;
            }
        }
    }
}