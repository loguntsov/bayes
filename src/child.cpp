#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "child.h"

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <signal.h>

using namespace std;

child::child(const char* exec) {

    this->stream = NULL;
    this->pid = 0;



    int outfd[2];
    int infd[2];

    int oldstdin, oldstdout;

    oldstdin = dup(STDIN_FILENO); // Save current stdin
    oldstdout = dup(STDOUT_FILENO); // Save stdout

    pipe(outfd); /* Where the parent is going to write to */
    pipe(infd); /* From where parent is going to read */

    fcntl (infd[0], F_SETFL, fcntl(infd[0], F_GETFL) | O_NONBLOCK);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);

    dup2(outfd[0], STDIN_FILENO); // Make the read end of outfd pipe as stdin
    dup2(infd[1], STDOUT_FILENO); // Make the write end of infd as stdout

    pid_t pid = fork();
    if(!pid) {

        close(outfd[0]); // Not required for the child
        close(outfd[1]);
        close(infd[0]);
        close(infd[1]);
        system(exec);
        exit(0);

    } else {

        close(STDIN_FILENO);
        close(STDOUT_FILENO); // Restore the original std fds of parent

        dup2(oldstdin, STDIN_FILENO);
        dup2(oldstdout, STDOUT_FILENO);

        close(outfd[0]); // These are being used by the child
        close(infd[1]);

        this->infd = infd[0];
        this->outfd = outfd[1];
        this->stream = new stringstream();
        this->pid = pid;

    }

}

child::~child() {
    delete this->stream;
    if (this->pid != 0) {
        kill(this->pid, SIGTERM);
    }
}

stringstream * child::get() {
    char input[150];
    bool is_cr = false;
    int n = 0, step = 0;
    do {
        n = read(this->infd,input,100);
        if (n>0) {
            step = 0;
            input[n] = 0;
            *this->stream << input;
            if (input[n-1] == 10) is_cr = true;
            if (n==1 && input[0] < 32) break;
            continue;
        }
        if (step > 1000 || is_cr) break;
        this->pause();
        step ++;
    } while (true);
    return this->stream;
}

void child::set(std::string text) {
    this->stream->str("");
    this->stream->clear();
    text += '\n';
    const char *str = text.c_str();
    write(this->outfd, str, strlen(str));
    this->pause();
}

stringstream * child::result(std::string text) {
    this->set(text);
    return this->get();
}

void child::pause() const {
    usleep(1000);
}
