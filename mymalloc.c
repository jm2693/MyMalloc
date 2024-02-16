#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
static char memory[MEMLENGTH];

typedef struct memory_chunk {
    int size;                       // size of memory chunk (including header)
    int in_use;                     // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)

    //struct memory_chunk *next;      // pointer to the next available free space 

} memory_chunk;

//malloc implementation 
void *mymalloc(size_t size, char *file, int line) {

    int i = 0;
    while(i == -1){
        if(memory[i] == NULL){
            
        }
        else i++;
    }

    return NULL;
}


void myfree(void *ptr, char *file, int line) {

}


