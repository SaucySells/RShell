#include "Semicolon.h"

Semicolon::Semicolon(string command, Command* prev):Command(command, prev) {
}


string Semicolon::getType() {
    return "semicolon";
}

void Semicolon::RedirectionAndPiping(char* args[], bool &commandFinished) {
    bool readFromPipe = false;
    while (args[0] != NULL) {
        //cout << "newloop" << endl;
        unsigned i = 0;
        bool piped = false;
        bool found = false;
        int file_desc = 0;
        pid_t ioPid = -1;
        int fd[2];
        unsigned endofCommand = 0;
        bool inputRedirect = false;
        for (i = 0; args[i] != NULL; ++i) {
            string ioString(args[i]);
            /* piping */
            if (ioString == "|") {
                //cout << "piperoni" << endl;
                found = true;
                piped = true;
                if (!readFromPipe) {
                    if (pipe(fd) < 0) {
                        //error
                        perror("pipe error");
                        exit(1);
                    }
                }
                /*if (readFromPipe) {
                    close(fd[1]);
                }*/
                if (ioPid == -1) {
                    ioPid = fork();
                }
                if (ioPid == -1) {
                    //fork failed
                    perror("fork failed");
                    exit(1);
                }
                else if (ioPid == 0) { //child pid
                    if (dup2(fd[1], 1) < 0) {
                        perror("Dup2 pipe Error");
                        exit(1);
                    }
                    if (readFromPipe) {
                        dup2(fd[0], 0);
                    }
                    //close(fd[0]);
                }
                commandFinished = true;
                if (endofCommand == 0) {
                    endofCommand = i;
                }
                break;
            }
            /* redirection */
            else if (ioString == "<") {
                //readFromPipe = false;
                found = true;
                inputRedirect = false;
                if (ioPid == -1) {
                    ioPid = fork();
                }
                if (ioPid == -1) {
                    //fork failed
                    perror("fork failed");
                    exit(1);
                }
                if (ioPid == 0) { //child pid
                    file_desc = open(args[i+1], O_RDONLY);
                    dup2(file_desc, 0);
                }
                commandFinished = true;
                if (endofCommand == 0) {
                    endofCommand = i;
                }
                //break;
            }
            else if (ioString == ">") {
                if (readFromPipe) {
                    close(fd[1]);
                }
                found = true;
                if (ioPid == -1) {
                    ioPid = fork();
                }
                if (ioPid == -1) {
                    //fork failed
                    perror("fork failed");
                    exit(1);
                }
                if (ioPid == 0) { //child pid
                    if (readFromPipe) {
                        //close(fd[1]);
                        dup2(fd[0], 0);
                    }
                    //cout << args[i+1] << endl;
                    file_desc = open(args[i+1], O_WRONLY | O_CREAT);
                    dup2(file_desc, 1);
                }
                commandFinished = true;
                if (endofCommand == 0) {
                    endofCommand = i;
                }
                break;
            }
            else if (ioString == ">>") {
                if (readFromPipe) {
                    close(fd[1]);
                }
                found = true;
                if (ioPid == -1) {
                    ioPid = fork();
                }
                if (ioPid == -1) {
                    //fork failed
                    perror("fork failed");
                    exit(1);
                }
                if (ioPid == 0) { //child pid
                    if (readFromPipe) {
                        //close(fd[1]);
                        dup2(fd[0], 0);
                    }
                    //close(fd[1]);
                    file_desc = open(args[i+1], O_WRONLY | O_APPEND | O_CREAT);
                    dup2(file_desc, 1);
                }
                commandFinished = true;
                if (endofCommand == 0) {
                    endofCommand = i;
                }
                break;
            }
            else if ((args[i+1] == NULL) && (!found) && (!inputRedirect)) {
                found = true;
                if (readFromPipe) {
                    close(fd[1]);
                }
                if (ioPid == -1) {
                    ioPid = fork();
                }
                if (ioPid == -1) {
                    //fork failed
                    perror("fork failed");
                    exit(1);
                }
                if (ioPid == 0) {
                    if (readFromPipe) {
                        close(fd[1]);
                        dup2(fd[0], 0);
                    }
                }
                commandFinished = true;
                if (endofCommand == 0) {
                    endofCommand = i+1;
                }
                break;
            }
        }
        if ((ioPid == 0) && (found)) { //child pid
            char* newArgs[200];
            unsigned j;
            for (j = 0; j < endofCommand; ++j) {
                newArgs[j] = args[j];
                //cout << newArgs[j] << endl;
            }
            newArgs[j] = NULL;
            if (execvp(newArgs[0], newArgs) == -1) {
                //command failed
                perror("command failed");
                exit(1);
            }
            else {
                exit(0);
            }
        }
        else if ((ioPid > 0) && (found)) { //parent id
            int status;
            bool run = true;
            
            while (run) {
                pid_t endID = waitpid(ioPid, &status, WNOHANG|WUNTRACED);
                if (endID == -1) {
                    perror("waitpid error");
                    exit(1);
                }
                else if (endID == 0) { //child process still running
                    //cout << "waiting" << endl;
                    usleep(10000);
                }
                else if (endID == ioPid) { //child ended
                    if (WIFEXITED(status)) {
                        if (status == 0) {
                            this->succeeded = true;
                        }
                        if (status == -1) {
                            this->succeeded = false;
                        }
                        run = false;
                    }
                    else if (WIFSTOPPED(status)) {
                        this->succeeded = false;
                        run = false;
                    }
                }
            }
        }
        usleep(10000);
        if (piped) {
            readFromPipe = true;
            //close(fd[1]);
            //close(fd[0]);
            unsigned k;
            unsigned q = 0;
            for (k = i+1; args[k] != NULL; ++k) {
                args[q] = args[k];
                ++q;
            }
            args[q] = NULL;
        }
        else {
            return;
        }
    }
}

void Semicolon::runCommand() {
    if (cmd == "exit") {
        exit(0);
    }
    this->ran = true;
    
    bool commandFinished = false;
    char* args[200];
    char* input = new char [this->cmd.length()+1]; 
    strcpy(input, this->cmd.c_str());
    char* pch = strtok(input, " ");
    args[0] = pch;
    int i = 0;
    while (pch != NULL) { // splits cmd into command and its arguments
        pch = strtok(NULL, " ");
        args[i+1] = pch;
        ++i;
    }
    args[i+1] = NULL;
    string arg0(args[0]);
    
    RedirectionAndPiping(args, commandFinished);
    if (commandFinished) {
        return;
    }
    if ((arg0 == "test") || (arg0 == "[")) {
        string flag;
        string arg1;
        if (args[1] != NULL) {
            arg1 += args[1];
        }
        else {
            arg1 = "-e";
        }
        if (((arg1 != "-e") && (arg1 != "-d") && (arg1 != "-f")) && (args[2] != NULL)) {
            perror("unexpected flag entered");
            return;
        }
        if (((arg1 != "-e") && (arg1 != "-d") && (arg1 != "-f")) || (args[2] == NULL)) {
            flag = "-e";
        }
        else if (arg1 == "-e") {
            flag = "-e";
        }
        else if (arg1 == "-d") {
            flag = "-d";
        }
        else if (arg1 == "-f") {
            flag = "-f";
        }
        struct stat buf;
        string path;
        if (args[2] == NULL && ((args[1] != NULL) && args[1] != arg1)) {
            path = arg1;
        }
        int i = 2;
        while (args[i] != NULL) {
            path += args[i];
            if (args[i+1] != NULL) {
                path += " ";
            }
            ++i;
        }
        if (path == "" && args[1] != NULL) {
            this->succeeded = true;
            return;
        }
        if (arg0 == "[") {
            path.erase(path.size()-2,2);
        }
        if (stat(path.c_str(), &buf) == 0) {
            if (S_ISDIR(buf.st_mode)) {
                this->succeeded = true;
                //file is directory
            }
            else if (S_ISREG(buf.st_mode)) {
                this->succeeded = true;
                //file is regular file
            }
            else {
                this->succeeded = false;
                //doesn't exist
            }
        }
        else {
            //file doesn't exist
            this->succeeded = false;
        }
        if (this->succeeded) {
            cout << "(True)" << endl;
        }
        else {
            cout << "(False)" << endl;
        }
        return;
        
    }
    pid_t pid = fork();
    if (pid == -1) {
        //fork failed
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) { //child pid
        if (execvp(args[0], args) == -1) {
            //command failed
            perror("command failed");
            exit(1);
        }
        else {
            exit(0);
            
        }
    }
    else if (pid > 0) { //parent id
        int status;
        bool run = true;
        
        while (run) {
            pid_t endID = waitpid(pid, &status, WNOHANG|WUNTRACED);
            if (endID == -1) {
                perror("waitpid error");
                exit(1);
            }
            else if (endID == 0) { //child process still running
                usleep(10000);
            }
            else if (endID == pid) { //child ended
                if (WIFEXITED(status)) {
                    if (status == 0) {
                        this->succeeded = true;
                    }
                    if (status == -1) {
                        this->succeeded = false;
                    }
                    run = false;
                }
                else if (WIFSTOPPED(status)) {
                    this->succeeded = false;
                    run = false;
                }
            }
        }
    }
    usleep(10000);
    return;
}