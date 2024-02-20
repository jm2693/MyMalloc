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

void *next_chunk(metadata *current_header) {                                                        // passes the current header to find location of next header
    char *next_ptr = (char*)(current_header + (current_header->chunk_size)/(sizeof(metadata)));     // second part gets size of current chunk in terms of metadata added to current header ptr and all casted as a char pointer for bytes
    if (next_ptr <= (char*)(&memory[MEMLENGTH-1])) {                                               // if 
        return (void*)next_ptr;                                                                     // returns a void pointer to the next header. Can be casted to metadata or int
    }
    return NULL;                                                                                    // if the next header ptr goes outside of the array it returns NULL
}

void init_next_chunk(metadata *current_header, size_t size) {   // takes ptr to current_header and size of (metadata+freespace)
    metadata *next_header = (next_chunk(current_header));      // uses next_chunk helper to get ptr to next chunk spot
    if (next_header != NULL) {                                  // checks to see if next chunk would be within array heap
        next_header->chunk_size = size;                                  // sets first spot of header to be size
        next_header->in_use = 0;                                     // sets second spot if in use (by default it is not since this is made after an allocation of the previous chunk)
    } 
}

void mergeChunks(int* current_header, int* nextChunk){
    current_header[0] = current_header[0] + nextChunk[0];
    nextChunk[0] = 0;
    nextChunk[1] = 0;
}


//malloc implementation 
void *mymalloc(size_t size, char *file, int line) {
    size = align(size);                                                         // ensures allignment 
    if(size > MEMLENGTH*sizeof(double)-sizeof(metadata)|| size <= 0){           // checks if size is bigger than 4096 bytes or less than or equal to 0 
        printf("Error at %s:%d: Invalid size\n", file, line);                   // error message
        return NULL;
    }
    
    metadata *chunk = (metadata*)memory;                        // storing size and if in_use within metadata struct
    double *payload = NULL;                                     // ptr to payload returned to client. initially points to nothing 
    double *start_ptr = memory;                                 // pointer to the start of memory  
    double *end_ptr = &memory[MEMLENGTH-1];                     // pointer to end of memory 

    while (start_ptr <= end_ptr) {                              // scans through entire heap array until it ends
        int *curr_header = (int*)start_ptr;                     // points to start of memory on first run. int pointer to get metadata values                                      
        chunk->chunk_size = curr_header[0];                                    
        chunk->in_use = curr_header[1];

        if (chunk->chunk_size == 0 && chunk->in_use == 0) {     // first metadata ints are 0, i.e. not allocated and size of 0 (not initialized)
            chunk->chunk_size = size + sizeof(metadata);        // allocated size asked for plus size of its own header
            chunk->in_use = 1;                                  // in_use = 1 to represent curr_header being allocated
            payload = start_ptr + 1;                            // increment current pointer to one following start
            printf("returning address of %p\n", payload);

            // use init_next_chunk here with curr_header and size of metadata+available space afterwards (in ints)
            init_next_chunk(chunk, (chunk->chunk_size - size));
            return (void*)payload;
            
        } else if (chunk->chunk_size - (size + sizeof(metadata)) >= 0) {         
            chunk->chunk_size = size + sizeof(metadata);        // allocated size asked for plus size of its own header
            chunk->in_use = 1;                                  // in_use = 1 to represent curr_header being allocated
            payload = start_ptr + 1;
            printf("returning address of %p\n", payload);

            // use init_next_chunk here with curr_header and size of metadata+available space afterwards (in ints)
            init_next_chunk(chunk, (chunk->chunk_size - size));
            return (void*)payload;
        } 

        //if (chunk->chunk_size - (size + sizeof(metadata)) == sizeof(metadata)) {
            
        //} 

        else if (chunk->chunk_size < (size + sizeof(metadata)) || chunk->in_use != 0) {
            if (start_ptr != NULL) {
                start_ptr = next_chunk((metadata*)(start_ptr));     
            } else break;
        }
    }
    printf("Error at %s:%d: Not enough memory :(\n", file, line);
    return NULL;
    
}


void myfree(void *ptr, char *file, int line) {
    char *start_ptr = (char *)memory;                                                    // casts pointers as char to read memory at byte level
    char *end_ptr = (char *)(&memory[MEMLENGTH-1]);                                      // points to end of memory 

    while(start_ptr <= end_ptr){
        metadata init;                                           
        int *chunk = (int*)start_ptr;                                                    // points to start of memory
        init.chunk_size = chunk[0];                                    
        init.in_use = chunk[1]; 

        if(init.in_use == 0 && (start_ptr + init.chunk_size + sizeof(metadata)) == ptr){ // checks for if the data is not allocated and if address is the same as pointer
            int *currentChunk = (int *)ptr - sizeof(metadata)/sizeof(int);               // points to metadata of chunk being deallocated
            if(currentChunk[1] == 0){                                                    // if it has been deallocated, give error message
                printf("Error at %s:%d: Freed this 2 memory already :(\n", file, line);           
                return;
            }
            int *nextChunk = next_chunk((metadata*)currentChunk);                        // set nextChunk to point to the next chunk of currentChunk
            if(nextChunk != NULL && nextChunk[1] == 0){                                  // 
                mergeChunks((int *)start_ptr, nextChunk);                                // merge start and the next chunk (which would be empty)
            }
            mergeChunks((int *)start_ptr, (int *)(char *)ptr - sizeof(metadata));        // merge 
            ptr = NULL;                                                                  // fully deallocated the ptr
            return;
        }
        
        else if(start_ptr + sizeof(metadata) == (char*)ptr){                                 // checks if the data is equal to the pointer
            int *currentChunk = (int *)ptr - sizeof(metadata)/sizeof(int);               // points to current chunk
            if(currentChunk[1] == 0){
                
                printf("Error at %s:%d: Freed this 1 memory already :(\n", file, line); 
                return;
            }
            int *nextChunk = next_chunk((metadata*)currentChunk);                                  
            if(nextChunk != NULL && nextChunk[1] == 0){                                
                mergeChunks((int *)start_ptr, nextChunk);                               
            }
            currentChunk[1] = 0;                                                         // marks current chunk as deallocated
            ptr = NULL;                                                                  // deallocates ptr
            return;
        }

        int *nextChunk = next_chunk((metadata*)(start_ptr));
        if (nextChunk != NULL) {
            start_ptr = (char*)nextChunk;
        } else break;
    }
    printf("Error at %s:%d: This pointer was not initialized :(\n", file, line);
}