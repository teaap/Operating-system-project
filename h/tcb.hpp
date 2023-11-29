//
// Created by os on 8/5/22.
//

#ifndef PROJECT_BASE_TCB_HPP
#define PROJECT_BASE_TCB_HPP
#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/printing.hpp"
class TCB
{
public:
    enum stanje{RUNNING,IDLE,CREATED,FINISHED,MAIN,WAIT};
    void *operator new(size_t size);
    void operator delete(void *ptr);

    static TCB* idle;
    static TCB* main;

    ~TCB() { delete stack; }
    static TCB* mainNit();
    bool isFinished() const { return finished; }
    void setFinished(bool value) { finished = value; }

    static void start(TCB *handle);
    static int thread_exit();

    using Body = void (*)(void*);
    static int idPoc;
    static void thread_dispatch();
    static TCB *thread_create_idle(Body body,void *arg,uint64 *stack);
    static TCB *running;
    static TCB *thread_create(Body body,void *arg,size_t *stack);

    TCB(){
        body= nullptr;
        stack= (uint64*)MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE);
        context={0,0};
        arg= nullptr;
        finished=false;
        running=main=this;
        stanje=MAIN;
    }
    stanje stanje;

private:
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    bool finished;
    void *arg;


    TCB(Body b,void* arg,size_t *stack) {
        this->body=b;
        this->stack=stack;
        context.ra=(uint64) &threadWrapper;
        context.sp = (uint64) &stack[DEFAULT_STACK_SIZE];
        this->arg=arg;
        finished=false;
    }

private:
    friend class Riscv;
    static void threadWrapper();
    static void contextSwitch(Context *oldContext, Context *runningContext);
};



#endif //PROJECT_BASE_TCB_HPP
