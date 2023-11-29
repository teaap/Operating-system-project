#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/userMain.h"



void userMain(void *arg) {
    printString("Korisnicki rezim\n\n");

    userMain();

    printString("\nKraj testa\n");
    thread_dispatch();
}

void idleFunkcija(void *arg){
    while(1){
    }
}

int main() {
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    Riscv::mc_sstatus(Riscv::SSTATUS_SPIE);
    void *s= mem_alloc(DEFAULT_STACK_SIZE);

    printString("Kernel rezim\n");
    TCB* t0=TCB::thread_create_idle(idleFunkcija,nullptr, (uint64*)s);
    void *arg=nullptr;
    thread_t t1= TCB::mainNit();
    thread_t t2=nullptr;
    thread_create(&t2,userMain,arg);
    thread_dispatch();
    printString("\nKraj kernel rezima \n");
    printString("Kraj! \n");
    delete t1;
    delete t2;
    delete t0;
   // MemoryAllocator::ispis();

    return 0;
}

