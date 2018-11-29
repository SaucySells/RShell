#include "Command.h"

Command::Command() {
    groupPos = -1;
}

Command::Command(string command, Command* prev) {
    cmd = command;
    this->prev = prev;
    succeeded = false;
    ran = false;
    groupPos = -1;
}

string Command::getCommand() {
    return cmd;
}

void Command::setCommand(string str) {
    cmd = str;
}

void Command::setPrev(Command* prv) {
    prev = prv;
}

void Command::setGroup(int num) {
    this->groupPos = num;
}

int Command::getGroup() {
    return groupPos;
}

bool Command::getSuccess() {
    return succeeded;
}

bool Command::getRan() {
    return ran;
}

Command* Command::getPrev() {
    return prev;
}

