#ifndef COMMANDGROUP_H
#define COMMANDGROUP_H

#include "Command.h"

class commandGroup : public Command {
    private:
        vector<Command*> inParenthesis;
        string connectorType;
    
    public:
        commandGroup(Command* prevCommand, string connector) {
            cmd = "isGroup";
            succeeded = false;
            prev = prevCommand;
            ran = false;
            connectorType = connector;
        }
        
        void addCommand(Command* command) {
            inParenthesis.push_back(command);
        }
        
        int numCommands() {
            return inParenthesis.size();
        }
        
        string ReturnCommand(int j) {
            if (j < inParenthesis.size()) {
                return inParenthesis.at(j)->getCommand();
            }
            else {
                return "";
            }
        }
        
        string getType() {
            return connectorType;
        }
        
        void runCommand() {
            if (inParenthesis.size() == 0) {
                return;
            }
            if ((connectorType == "none") || (connectorType == "semicolon")) {
                
            }
            else {
                while (prev != NULL) {
                    if (prev->getRan()) {
                        break;
                    }
                    else {
                        Command* temp = prev;
                        prev = temp->getPrev();
                    }
                }
                if (connectorType == "and") {
                    if (this->prev->getSuccess() == false) {
                        // || type command can't run
                        this->succeeded = false;
                        return;
                    }
                }
                if (connectorType == "or") {
                    if (this->prev->getSuccess() == true) {
                        // || type command can't run
                        this->succeeded = false;
                        return;
                    }
                }
            }
            this->ran = true;
            Command* lastRun = NULL;
            for (unsigned i = 0; i < inParenthesis.size(); ++i)  {
                for (unsigned j = i+1; j < inParenthesis.size(); ++j) {
                    if (inParenthesis.at(i)->getPrev() != lastRun) {
                        Command* temp = inParenthesis.at(i);
                        inParenthesis.at(i) = inParenthesis.at(j);
                        inParenthesis.at(j) = temp;
                    }
                }
                lastRun = inParenthesis.at(i);
                cout << "order" << endl;
                cout << inParenthesis.at(0)->getCommand() << endl;
                cout << inParenthesis.at(1)->getCommand() << endl;
                cout << "end order" << endl;
                //cout << inParenthesis.at(2)->getCommand() << endl;
            }
            
            for (unsigned i = 0; i < inParenthesis.size(); ++i) {
                inParenthesis.at(i)->runCommand();
            }
            if (inParenthesis.back()->getType() != "or") {
                this->succeeded = inParenthesis.back()->getSuccess();
            }
            else {
                for (unsigned i = 0; i < inParenthesis.size(); ++i) {
                    if (inParenthesis.at(i)->getSuccess()) {
                        this->succeeded = true;
                        i = inParenthesis.size();
                    }
                }
            }
        }

};

#endif 