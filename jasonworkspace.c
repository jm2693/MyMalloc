#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
static double memory[MEMLENGTH];    // defines heap 



typedef struct memory_chunk {
    int size;                       // size of memory chunk (including header)
    int in_use;                     // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)

    //struct memory_chunk *next;      // pointer to the next available free space 

} memory_chunk;

size_t align(size_t size) {         // method to align everything as 8-byte aligned
    return (size+7) & ~7;           // uses addition and bitwise and to round up to nearest multiple of 8
}


void init_heap() {
    
}


void *mymalloc(size_t size, char *file, int line) {
    

}


void myfree(void *ptr, char *file, int line) {

}




