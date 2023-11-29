//
// Created by os on 7/25/22.
//

#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/printing.hpp"

struct BlockHeader{
    BlockHeader* next= nullptr;
    size_t size;
};

struct FreeBlockHeader{
    FreeBlockHeader* next= nullptr;
    size_t size;
};

static FreeBlockHeader *freeMemHead= (FreeBlockHeader*)HEAP_START_ADDR;
static BlockHeader *memHead= nullptr;
size_t ZAGL =sizeof(BlockHeader);

void* MemoryAllocator::mem_alloc(size_t size){
    size_t velicina=0;
    if(size%(long unsigned int)MEM_BLOCK_SIZE==0) velicina=size;
    else velicina=(size/(long unsigned int)MEM_BLOCK_SIZE+1)*(long unsigned int)MEM_BLOCK_SIZE;
    if(memHead==nullptr){
        BlockHeader *novi=(BlockHeader*)(HEAP_START_ADDR);
        novi->size=velicina+ZAGL;
        novi->next=nullptr;
        memHead=novi;
        freeMemHead=(FreeBlockHeader*)((unsigned long int)novi+velicina+ZAGL);
        freeMemHead->next=nullptr;
        freeMemHead->size=((long unsigned int)HEAP_END_ADDR-(long unsigned int)HEAP_START_ADDR)-novi->size;
        if(freeMemHead->size<0) return nullptr;
        return (void*)((size_t)novi+ZAGL);
    }
    FreeBlockHeader *blk=freeMemHead,*prev=nullptr;
    BlockHeader *zauzeti=memHead,*prethodni=nullptr;
    void* povratna=nullptr;
    while(blk!=nullptr){
        FreeBlockHeader* sledeci=blk->next;
        if(blk->size>=velicina+ZAGL) {
            blk->size -= (velicina+ZAGL);
            povratna=(void*)((unsigned long int)blk+ZAGL);
            if(blk->size>0) {
                FreeBlockHeader *novii = (FreeBlockHeader *) ((unsigned long int) blk + velicina + ZAGL);
                novii->size = blk->size;
                if (blk != freeMemHead)
                    prev->next = novii;
                else
                    freeMemHead = novii;
                novii->next = blk->next;
            }
            else
            {
                if(blk==freeMemHead) freeMemHead=blk->next;
                else {prev->next = blk->next;}
            }
            BlockHeader *novi= (BlockHeader*)((unsigned long int)povratna-ZAGL);
            novi->size=velicina+ZAGL;
            novi->next=nullptr;
            while(zauzeti!=nullptr){
                BlockHeader* sled=zauzeti->next;
                if((unsigned long int)novi<(unsigned long int)zauzeti){
                    if(zauzeti==memHead)
                    {
                        memHead=novi;
                    }
                    else prethodni->next=novi;
                    novi->next=zauzeti;
                    return (void*)((unsigned long int)novi+ZAGL);
                }
                prethodni=zauzeti;
                zauzeti=sled;
            }
            prethodni->next=novi;
            return (void*)((unsigned long int)novi+ZAGL);
        }
        prev=blk;
        blk=sledeci;
    }
    return nullptr;
}

int MemoryAllocator::mem_free(void *ptr) {
    bool test=false;
    //obrisi();
    // BlockHeader *pttr=(BlockHeader*)((unsigned long int)ptr-sizeof(size_t));
    BlockHeader *blk=memHead,*pret=nullptr;
    FreeBlockHeader *free=nullptr,*zauzeti=freeMemHead,*prethodni= nullptr;
    blk=memHead;
    while(blk!=nullptr){
        free=(FreeBlockHeader*)blk;
        if((unsigned long int)blk==((unsigned long int)ptr-16)){
            test=true;
            free->size=blk->size;                    //2147523648 2147523872
            if(blk==memHead){
                memHead=blk->next;}
            else{
                pret->next=blk->next;}
            break;
        }
        pret=blk;
        blk=blk->next;
    }
    if(test==false)
        return -1;
    while(zauzeti!= nullptr){
        if((unsigned long int)free<(unsigned long int)zauzeti){
            free->next=zauzeti;
            if(zauzeti!=freeMemHead) {
                prethodni->next = free;
            }
            else {
                freeMemHead = free;
            }
            prespoj();
            return 0;
        }
        prethodni=zauzeti;
        zauzeti=zauzeti->next;
    }
    free->next=nullptr;
    prethodni->next=free;
    if(blk==memHead)
        memHead=blk->next;
    else
        pret->next=blk->next;
    prespoj();
    return 0;
}

void MemoryAllocator::prespoj() {
    FreeBlockHeader *pocetni=freeMemHead,*sled=nullptr;
    while(pocetni!=nullptr){
        if(pocetni!=freeMemHead){
            if((size_t)sled+(size_t)sled->size==(size_t)pocetni){
                sled->size+=pocetni->size;
                sled->next=pocetni->next;
                pocetni= sled;
            }
        }
        sled=pocetni;
        pocetni=pocetni->next;
    }
}

void MemoryAllocator::ispis() {
    __putc('\n');
    for(BlockHeader *blk=memHead;blk!= nullptr;blk=blk->next) {
        printInt((unsigned long int) blk);
        __putc(' ');
        printInt(blk->size);
        __putc('\n');
    }
    printString("FREE");
    __putc('\n');
    for(FreeBlockHeader *blk=freeMemHead;blk!= nullptr;blk=blk->next) {
        printInt((unsigned long int) blk);
        __putc(' ');
        printInt(blk->size);
        __putc('\n');
    }
}