#ifndef UCPP_H
#define UCPP_H

#ifndef UCPP_COMPILER

#include <istream>
#include <ostream>

#define _Coroutine class
#define _Event class
#define _Task class
#define _Monitor class
#define _Cormonitor class
#define _Enable
#define _CatchResume catch
#define _Resume throw
#define _At ; throw
#define _Throw throw

#define COROUTINE \
    void resume();\
    void suspend();

#define TASK \
    void resume();\
    void suspend();\
    static void yield(unsigned int times = 1);\
    public: \
    enum State{Start, Ready, Running, Blocked, Terminate};\
    State getState() const;\
    private:

_Task uMain {
private:
    int argc;
    char **argv;
    int &uRetCode;
    void main();
};

struct uProcessor {};

class uSpinLock {
public:
    uSpinLock();
};

class uLock {
public:
    uLock(unsigned int value=1);
    void acquire();
    bool tryacquire();
    void release();
};

class uOwnerLock {
public:
    uOwnerLock() {}
    uBaseTask *owner();
    unsigned int times();
    void acquire();
    bool tryacquire();
    void release();
};

class uCondLock {
public:
    uCondLock();
    bool empty();
    void wait(uOwnerLock &lock);
    void signal();
    void broadcast();
};

class osacquire {
public:
    osacquire(std::ostream &out);
};

class isacquire {
public:
    isacquire(std::istream &in);
};

#else

#include "uC++.h"
#define COROUTINE
#define TASK

#endif // UCPP_COMPILER

#endif // UCPP_H
