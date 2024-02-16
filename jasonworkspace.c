#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
static double memory[MEMLENGTH];    // defines heap 



typedef struct memory_chunk {
    int size;                       // size of memory chunk (including header)
    int in_use;                     // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)

    //struct memory_chunk *next;      // pointer to the next available free space 

} memory_chunk;



void *mymalloc(size_t size, char *file, int line) {

}


void myfree(void *ptr, char *file, int line) {

}




