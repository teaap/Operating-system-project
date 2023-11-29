//
// Created by os on 7/24/22.
//

#ifndef _syscall_cpp
#define _syscall_cpp

#include "../h/syscall_c.h"

void* operator new (size_t);
void operator delete (void*);

class Thread {
public:
    static void wrapper(void *arg);
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif