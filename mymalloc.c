#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 512
static double memory[MEMLENGTH];

typedef struct metaData{
    int size;                       // size of memory chunk (including header)
    int in_use;                     // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)
    struct metaData *next;          // pointer to the next available free space 

} metaData;


size_t align(size_t size) {         // method to align everything as 8-byte aligned
    return (size+7) & ~7;           // uses addition and bitwise and to round up to nearest multiple of 8
}


void init_heap() {
    metaData *init_chunk = (metaData*)memory;                   // creating a metaData pointer to point to memory
    init_chunk->size = MEMLENGTH;                               // size of initial chunk is entire heap (including header)
    init_chunk->in_use = 0;                                     // initially has nothing allocated
}

//malloc implementation 
void *mymalloc(size_t size, char *file, int line) {
    size = align(size);                                         // ensures allignment 
    char *heap = (char*)memory;                                 // pointer to the start of memory for byte-width operations
    if(memory[0] != '/0'){                                      // checks if memory is initiazlied, proceeds to other checks and allocation 
        if(size > MEMLENGTH || size <= 0){                      // checks if size is bigger than 512 or less than or equal to 0 
            printf("Error: Invalid size\n");
        }
        else{

        }                   

    }                                  
    else init_heap();                                           // if its not initialized, call init_heap() to initialize
    

    
}


void myfree(void *ptr, char *file, int line) {

}


