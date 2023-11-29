//
// Created by os on 7/25/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"
class MemoryAllocator{
public:
    static void* mem_alloc(size_t size);

    static void ispis();

    static void prespoj();

    static int mem_free(void* ptr);

};

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP