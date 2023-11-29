//
// Created by os on 8/13/22.
//

#ifndef PROJECT_BASE_SYSCALL_C_H
#define PROJECT_BASE_SYSCALL_C_H

#include "../lib/hw.h"
#include "../h/tcb.hpp"

typedef TCB* thread_t;
    void* mem_alloc(size_t size);
    int mem_free(void* ptr);

    static void prepakuj();

    void thread_dispatch();
    int thread_create(thread_t *handle,void(*start_routine)(void*),void* arg);
    int thread_create_nostart(thread_t *handle,void(*start_routine)(void*),void* arg);
    int thread_start (thread_t ptr);
    int thread_exit();

    char getc();
    void putc(char c);

    enum kod{
        MEM_ALLOC=0x01,
        MEM_FREE=0x02,
        THREAD_CREATE=0x11,
        THREAD_EXIT=0x12,
        THREAD_DISPATCH=0x13,
        THREAD_CREATE_NOSTART=0x14,
        THREAD_START=0x15,
        GETC=0x41,
        PUTC=0x42
    };

    inline void prepakuj(){
        __asm__ volatile ("mv s8, a0");
        __asm__ volatile ("mv s9, a1");
        __asm__ volatile ("mv s7, a2");
    }




#endif //PROJECT_BASE_SYSCALL_C_H
