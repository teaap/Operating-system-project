//
// Created by os on 8/5/22.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP
#include "../h/list.hpp"

class TCB;
class Scheduler
{
private:
    static List<TCB> readyThreadQueue;
    static int cnt;

public:
    static TCB *get();

    static int counter();

    static void put(TCB *ccb);

    static void getLast();

    static TCB *peak();

};


#endif //PROJECT_BASE_SCHEDULER_HPP
