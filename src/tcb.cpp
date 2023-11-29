#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/printing.hpp"
#include "../lib/mem.h"

TCB *TCB::running = nullptr;
TCB *TCB::idle=nullptr;
TCB *TCB::main=nullptr;

void *TCB::operator new(size_t size) {return MemoryAllocator::mem_alloc(size); }
void TCB::operator delete(void *ptr) {MemoryAllocator::mem_free(ptr); }

TCB *TCB::thread_create(Body body,void *arg,size_t *stack)
{
  TCB* handle=nullptr;
  handle=new TCB(body,arg,stack);
  handle->stanje=CREATED;
  return handle;
}

void TCB::start(TCB *handle) {
   // handle->stanje=CREATED;
    Scheduler::put((handle));
}

int TCB::thread_exit() {
    running->stanje=FINISHED;
    thread_dispatch();
    return 0;
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

TCB *TCB::thread_create_idle(Body body, void *arg,uint64 *stack) {
    TCB *pov = thread_create(body,arg ,stack);
    pov->stanje=IDLE;
    TCB::idle=pov;
    return pov;
}

TCB *TCB:: mainNit(){
    main= new TCB();
    return main;
}

void TCB::thread_dispatch()
{
    TCB *old = running;
    if(old->stanje!=FINISHED && old->stanje!=WAIT&& old->stanje!=MAIN) {
        old->stanje = CREATED;
        Scheduler::put(old);
    }
    if(old->stanje==FINISHED) { delete old;}
    running= nullptr;
    running = Scheduler::get();
    if (!running) running=main;
    else running->stanje = RUNNING;
    Riscv::promeni();
    TCB::contextSwitch(&old->context, &running->context);
}

