//
// Created by os on 8/1/22.
//

#include "../h/syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/printing.hpp"

void *operator new(size_t size) {return mem_alloc(size); }
void  operator delete(void *ptr) {mem_free(ptr); }

Thread::~Thread() {
   delete myHandle;
}

Thread::Thread() {
    thread_create_nostart(&myHandle, wrapper, this);
}

Thread::Thread(void (*body)(void *), void *arg)  {
    thread_create_nostart(&myHandle, body, arg);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::start() {
    return thread_start(Thread::myHandle);
}

void Thread::wrapper(void *arg)
{
    Thread* thread = (Thread*)arg;
    thread->run();
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
