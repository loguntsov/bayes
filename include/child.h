#ifndef CHILD_H
#define CHILD_H

#include <string>
#include <sstream>

class child
{
    public:
        child(const char *exec);
        virtual ~child();
        std::stringstream * get();
        void set(std::string text);
        std::stringstream * result(std::string text);
    protected:
        pid_t pid;
        int infd;
        int outfd;
        std::stringstream *stream;
        void pause() const;
    private:
};

#endif // CHILD_H
