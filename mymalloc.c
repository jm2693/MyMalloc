#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define MEMLENGTH 512
static double memory[MEMLENGTH];

typedef struct metaData{
    int size;                       // size of memory chunk (including header)
    int in_use;                     // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)
    //struct metaData *next;          // pointer to the next available free space 
} metaData;

int get_size(metaData *ptr) {
    return ptr->size;               // return size of metadata+payload
}

size_t get_in_use(metaData *ptr) {
    return ptr->in_use;                 // returns in_use flag
}


size_t align(size_t size) {         // method to align everything as 8-byte aligned
    return (size+7) & ~7;           // uses addition and bitwise and to round up to nearest multiple of 8
}


void init_heap() {
    metaData *init_chunk = (metaData*)memory;                   // creating a metadata pointer to point to memory
    init_chunk->size = MEMLENGTH-sizeof(metaData);              // size of initial chunk is entire heap (including header)
    init_chunk->in_use = 0;                                     // initially has nothing allocated
}

//malloc implementation 
void *mymalloc(size_t size, char *file, int line) {
    if(size > MEMLENGTH || size <= 0){                      // checks if size is bigger than 4096 bytes or less than or equal to 0 
        printf("Error: Invalid size\n");
        return NULL;
    }
    size = align(size);                                         // ensures allignment 
    
    double *heap_ptr = memory;                                  // pointer to the start of memory  
    double *end_ptr = &memory[MEMLENGTH-1];                     // pointer to end of memory 

    while (heap_ptr <= end_ptr) {                               // scans through entire heap array until it ends
        
    }
    
}


void myfree(void *ptr, char *file, int line) {

}