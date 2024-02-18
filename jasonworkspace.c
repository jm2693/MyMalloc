#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define MEMLENGTH 512
static double memory[MEMLENGTH];

typedef struct metadata{
    int chunk_size;                 // size of memory chunk (including header)
    int in_use;                     // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)
    //struct metadata *next;        // pointer to the next available free space  (not going with this method because increases memory usage by a lot and not necessary for small data sizes)
} metadata;

size_t align(size_t size) {         // method to align everything as 8-byte aligned
    return (size+7) & ~7;           // uses addition and bitwise and to round up to nearest multiple of 8
}

void *next_chunk(metadata *current_header) {                       // passes the current header to find location of next header
    char *next_ptr = (char*)(current_header + (current_header->chunk_size)/(sizeof(metadata)));
    if (next_ptr-(char*)(MEMLENGTH+sizeof(metadata)) < 0) {
        return (void*)next_ptr;
    }
    return NULL;
}

void init_next_chunk(int *current_header) {

}


//malloc implementation 
void *mymalloc(size_t size, char *file, int line) {
    if(size > MEMLENGTH || size <= 0){                          // checks if size is bigger than 4096 bytes or less than or equal to 0 
        printf("Error at %s:%d: Invalid size\n", file, line);
        return NULL;
    }
    size = align(size);                                         // ensures allignment 
    
    metadata *chunk = (metadata*)memory;                        // storing size and if in_use within metadata struct
    double *payload = NULL;                                     // ptr to payload returned to client. initially points to nothing 
    double *start_ptr = memory;                                 // pointer to the start of memory  
    double *end_ptr = &memory[MEMLENGTH-1];                     // pointer to end of memory 

    while (start_ptr <= end_ptr) {                              // scans through entire heap array until it ends
        int *curr_header = (int*)start_ptr;                     // points to start of memory on first run. int pointer to get metadata values                                      
        chunk->chunk_size = curr_header[0];                                    
        chunk->in_use = curr_header[1];

        if (chunk->chunk_size == 0 && chunk->in_use == 0) {     // first metadata ints are 0, i.e. not allocated and size of 0 (not initialized)
            curr_header[0] = size + sizeof(metadata);           // allocated size asked for plus size of its own header
            curr_header[1] = 1;                                 // in_use = 1 to represent curr_header being allocated
            payload = start_ptr + 1;                            // increment current pointer to one following start
            return payload;
            
        } 
        if (chunk->chunk_size - size >= 0) {         
            curr_header[0] = size + sizeof(metadata);                  // allocated size asked for plus size of its own header
            curr_header[1] = 1;                                        // in_use = 1 to represent curr_header being allocated
            payload = start_ptr + 1;
            return payload;
            
        }

        start_ptr = next_chunk;
    }
    
}


void myfree(void *ptr, char *file, int line) {

}




// int get_size(metadata ptr) {
//     metadata *get_size = &ptr;
//     return get_size->size;               // return size of metadata+payload
// }

// size_t get_in_use(metadata *ptr) {
//     return ptr->in_use;                 // returns in_use flag
// }
