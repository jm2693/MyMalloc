#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define MEMLENGTH 512
static double memory[MEMLENGTH];

typedef struct metadata{
    int size;                       // size of memory chunk (including header)
    int in_use;                     // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)
    //struct metadata *next;        // pointer to the next available free space  (not going with this method because increases memory usage by a lot and not necessary for small data sizes)
} metadata;


// int get_size(metadata ptr) {
//     metadata *get_size = &ptr;
//     return get_size->size;               // return size of metadata+payload
// }

// size_t get_in_use(metadata *ptr) {
//     return ptr->in_use;                 // returns in_use flag
// }


size_t align(size_t size) {                                      // method to align everything as 8-byte aligned
    return (size+7) & ~7;                                        // uses addition and bitwise and to round up to nearest multiple of 8
}

void init_heap() {
    metadata *init_chunk = (metadata*)memory;                    // creating a metadata pointer to point to memory
    init_chunk->size = MEMLENGTH-sizeof(metadata);               // size of initial chunk is entire heap (including header)
    init_chunk->in_use = 0;                                      // initially has nothing allocated
}

//malloc implementation 
void *mymalloc(size_t size, char *file, int line) {
    if(size > MEMLENGTH || size <= 0){                           // checks if size is bigger than 4096 bytes or less than or equal to 0 
        printf("Error at %s:%d: Invalid size\n", file, line);
        return NULL;
    }
    size = align(size);                                          // ensures allignment 
    
    double *payload = NULL;
    double *start_ptr = memory;                                  // pointer to the start of memory  
    double *end_ptr = &memory[MEMLENGTH-1];                      // pointer to end of memory 

    while (start_ptr <= end_ptr) {                               // scans through entire heap array until it ends
        int *chunk = (int*)start_ptr;                            // points to start of memory on first run. int pointer to get metadata values
        metadata init;                                           // storing size and if in_use within metadata struct
        init.size = chunk[0];                                    
        init.in_use = chunk[1];

        if(init.size == 0 && init.in_use == 0) {                 // first metadata ints are 0, i.e. not allocated and size of 0 (not initialized)
            chunk[0] = size + sizeof(metadata);                  // allocated size asked for plus size of its own header
            chunk[1] = 1;                                        // in_use = 1 to represent chunk being allocated
            payload = start_ptr + 1;                             // increment current pointer to one following start
            
        } 
        else if (init.size >= size + sizeof(metadata) && init.in_use == 0) {         
            chunk[0] = size + sizeof(metadata);                  // allocated size asked for plus size of its own header
            chunk[1] = 1;                                        // in_use = 1 to represent chunk being allocated
                                                                  
        }                                                        
    }
    printf("Error at %s:%d: Not enough memory :(\n", file, line);
    return NULL;
    
}


void myfree(void *ptr, char *file, int line) {
    char *start_ptr = (char *)memory;                            // casts pointers as char to read memory at byte level
    char *end_ptr = (char *)(&memory[MEMLENGTH-1]);              // points to end of memory 

    while(start_ptr <= end_ptr){
        int *chunk = (int*)start_ptr;                            // points to start of memory
        metadata init;                                           
        init.size = chunk[0];                                    
        init.in_use = chunk[1];

        if(init.in_use == 0 && (start_ptr + init.size + 8) == ptr){
            int *currentChunk = (int *)ptr - sizeof(metadata)/sizeof(int);
            if(currentChunk[1] == 0){
                printf("Error at %s:%d: Freed this already :(\n", file, line);
                return;
            }
            
        }

        
    }
}