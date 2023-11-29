//
// Created by os on 7/26/22.
//

#include "../h/syscall_c.h"
#include "../h/riscv.hpp"

    void *mem_alloc(size_t size) {
        size_t novaSize = 0,pa;
        novaSize=(size/(long unsigned int)MEM_BLOCK_SIZE+1);
        __asm __volatile("mv a1,%0"::"r" (novaSize));
        __asm __volatile("mv a0,%0"::"r" (MEM_ALLOC));
        __asm __volatile("ecall");
        __asm__ volatile ("mv %0, a0" : "=r" (pa));
        return (void *) pa;
    }

    int mem_free(void *p) {
        size_t pa;
        __asm __volatile("mv a1,a0");
        __asm __volatile("mv a0,%0"::"r" (MEM_FREE));
        __asm __volatile("ecall");
        __asm__ volatile ("mv %0, a0" : "=r" (pa));
        return (int) pa;
    }

    int thread_start(thread_t p) {
        size_t pa;
        __asm __volatile("mv a1,a0");
        __asm __volatile("mv a0,%0"::"r" (THREAD_START));
        __asm __volatile("ecall");
        __asm__ volatile ("mv %0, a0" : "=r" (pa));
        return (int) pa;
    }

    int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg) {
        size_t pa;
        prepakuj();
        mem_alloc(DEFAULT_STACK_SIZE);
        __asm __volatile("mv a4, a0");
        __asm__ volatile ("mv a1, s8");
        __asm__ volatile ("mv a2, s9");
        __asm__ volatile ("mv a3, s7");
        __asm __volatile("mv a0,%0"::"r" (THREAD_CREATE));
        __asm __volatile("ecall");
        __asm__ volatile ("mv %0, a0" : "=r" (pa));
        if (pa == 0) return -1;
        return 0;
    }

    int thread_create_nostart(thread_t *handle, void(*start_routine)(void *), void *arg) {
        size_t pa;
        prepakuj();
        mem_alloc(DEFAULT_STACK_SIZE);
        __asm __volatile("mv a4, a0");
        __asm__ volatile ("mv a1, s8");
        __asm__ volatile ("mv a2, s9");
        __asm__ volatile ("mv a3, s7");
        __asm __volatile("mv a0,%0"::"r" (THREAD_CREATE_NOSTART));
        __asm __volatile("ecall");
        __asm__ volatile ("mv %0, a0" : "=r" (pa));
        if (pa== 0) return -1;
        return 0;
    }

    int thread_exit() {
        size_t pa;
        __asm __volatile("mv a0,%0"::"r" (THREAD_EXIT));
        __asm __volatile("ecall");
        __asm__ volatile ("mv %0, a0" : "=r" (pa));
        return (int) pa;
    }

    void thread_dispatch() {
        __asm __volatile("mv a0,%0"::"r" (THREAD_DISPATCH));
        __asm __volatile("ecall");
    }

    char getc(){
        size_t pa;
        __asm __volatile("mv a0,%0"::"r" (GETC));
        __asm __volatile("ecall");
        __asm__ volatile ("mv %0, a0" : "=r" (pa));
        return (char)pa;
    }

    void putc(char c){
        __asm __volatile("mv a1,a0");
        __asm __volatile("mv a0,%0"::"r" (PUTC));
        __asm __volatile("ecall");
    }

