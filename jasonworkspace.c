#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
static double memory[MEMLENGTH];    // defines heap 



typedef struct metadata {
    size_t size;                    // size of memory chunk (including header)
    int in_use;                     // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)
    struct metadata *next;          // pointer to the next available free space 

} metadata;

int get_size(metadata *ptr) {
    return ptr->size;
}

size_t get_in_use(metadata *ptr) {
    return ptr->in_use;
}


size_t align(size_t size) {         // method to align everything as 8-byte aligned
    return (size+7) & ~7;           // uses addition and bitwise and to round up to nearest multiple of 8
}


void init_heap() {
    metadata *init_chunk = (metadata*)memory;           // creating a metadata pointer to point to memory
    init_chunk->size = MEMLENGTH-sizeof(metadata);                               // size of initial chunk is entire heap (including header)
    init_chunk->in_use = 0;                                     // initially has nothing allocated
}



void *mymalloc(size_t size, char *file, int line) {

    size = align(size);                                         // ensures allignment 

    char *heap_ptr = (char*)memory;                             // pointer to the start of memory    
    // check to see if (metadata*)memory size is something and in use is 0
    // if yes allocate something
    // if no call init-heap

}


void myfree(void *ptr, char *file, int line) {

    if (ptr == NULL) return;                                    // needs to return an error statements

    // coalece 

}




