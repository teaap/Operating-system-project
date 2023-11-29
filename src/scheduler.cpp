
#include "../h/scheduler.hpp"

List<TCB> Scheduler::readyThreadQueue;
int Scheduler::cnt;

TCB *Scheduler::get()
{
    cnt--;
    return readyThreadQueue.removeFirst();
}

int Scheduler::counter() {
    return cnt;
}

void Scheduler::put(TCB *ccb)
{
    readyThreadQueue.addLast(ccb);
    cnt++;
}

void Scheduler::getLast()
{
    readyThreadQueue.removeLast();
}

TCB *Scheduler::peak()
{
    return readyThreadQueue.peekFirst();
}