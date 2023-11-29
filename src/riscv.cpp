#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/printing.hpp"
#include "../h/syscall_c.h"

void Riscv::popSppSpie()
{
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::promeni(){
    ms_sstatus(SSTATUS_SPP);
}

void Riscv::handleSupervisorTrap()
{
    Riscv::argumenti();
    size_t kod;
    __asm__ volatile ("mv %0, s1" : "=r" (kod));

    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        uint64 volatile sepc = r_sepc() + 4;
        switch(kod) {
            case MEM_ALLOC:{
                int cnt=0;
                __asm__ volatile ("mv %0, s2" : "=r" (cnt));
                size_t ret = (size_t) MemoryAllocator::mem_alloc(cnt* MEM_BLOCK_SIZE);
                povratna(ret);
                break;
            }
            case MEM_FREE:{
                void *param;
                __asm__ volatile ("mv %0, s2" : "=r" (param));
                size_t ret = (size_t) MemoryAllocator::mem_free(param);
                povratna(ret);break;
            }
            case THREAD_CREATE:{
                TCB::Body b;thread_t* handle;size_t *stek;void *arg;
                __asm__ volatile ("mv %0, s2" : "=r" (handle));
                __asm__ volatile ("mv %0, s3" : "=r" (b));
                __asm__ volatile ("mv %0, s4" : "=r" (arg));
                __asm__ volatile ("mv %0, s5" : "=r" (stek));
                *handle=TCB::thread_create(b,arg,stek);
                TCB::start(*handle);
                size_t pov=0;
                if(handle==nullptr) pov=-1;
                povratna(pov);
                break;
            }
            case THREAD_EXIT:{
                size_t ret = (size_t) TCB::thread_exit();
                povratna(ret);break;
            }
            case THREAD_DISPATCH:{
                TCB::thread_dispatch();
                break;
            }
            case THREAD_CREATE_NOSTART:{
                TCB::Body b;thread_t* handle;size_t *stek;void *arg;
                __asm__ volatile ("mv %0, s2" : "=r" (handle));
                __asm__ volatile ("mv %0, s3" : "=r" (b));
                __asm__ volatile ("mv %0, s4" : "=r" (arg));
                __asm__ volatile ("mv %0, s5" : "=r" (stek));
                *handle=TCB::thread_create(b,arg,stek);
                size_t pov=0;
                if(handle==nullptr) pov=-1;
                povratna(pov);
                break;
            }
            case THREAD_START:{
                thread_t handle;
                __asm__ volatile ("mv %0, s2" : "=r" (handle));
                TCB::start(handle);
                size_t pov=0;
                if(handle==nullptr) pov=-1;
                povratna(pov);
                break;
            }
            case GETC: {
                char pa=__getc();
                povratna((size_t)pa);
                break;
            }
            case PUTC:{
                char handle;
                __asm__ volatile ("mv %0, s2" : "=r" (handle));
                __putc(handle);
                break;
            }
        }
        w_sepc(sepc);

    }
    else if (scause == 0x8000000000000001UL)
    {
        uint64 volatile sstatus = r_sstatus();
        mc_sip(SIP_SSIP);
        w_sstatus(sstatus);

    }
    else if (scause == 0x8000000000000009UL)
    {
        console_handler();
    }
    else
    {
        int sepc=Riscv::r_sepc();
        int stval=Riscv::r_stval();
        printString("\n");
        printString("scause: ");
        printInt(scause);
        printString("\n");
        printString("sepc=");
        printInt(sepc);
        printString(" stval=");
        printInt(stval);printString("\n");thread_exit();return;
    }
}