#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#ifndef DEBUG
#define DEBUG 0
#endif
#define MEMLENGTH 512
static double memory[MEMLENGTH];

typedef struct metadata{
    int size;                           // size of memory chunk (including header)
    int use;                            // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)
} metadata;

size_t align(size_t size) {                 // method to align everything as 8-byte aligned
    return (size+7) & ~7;                   // uses addition and bitwise and to round up to nearest multiple of 8
}

void *find_next_chunk(int* current) {                                                        // takes current beginning of chunk as an int ptr to parse through the int data within the struct
    int* next_chunk = (int *)(current + current[0]/sizeof(int));                             // the next space in memory will be the current chunk + the size of the chunk itself (metadata+payload)
    if(next_chunk <= (int *)(&memory[MEMLENGTH - (sizeof(metadata)/sizeof(double))])) {      // as long as next_chunk is wthin memory it returns next chunk
        return (void *)next_chunk;
    } 
    return NULL;
}

void init_next_chunk(int* curr_header, size_t size) {       // takes ptr to current_header and size of (metadata+freespace)
    int* nextcurr_header = find_next_chunk(curr_header);    // uses find_next_chunk helper to get ptr to next chunk spot
    if(nextcurr_header != NULL) {                           // checks to see if next chunk would be within array heap
        nextcurr_header[0] = size;                          // sets first spot of header to be size
        nextcurr_header[1] = 0;                             // sets second spot if in use (by default it is not since this is made after an allocation of the previous chunk)
    }
}

void assign_header(int* current_headerstart, size_t size) {     // used to set the data within the next header 
    current_headerstart[0] = size;                              // sets the size of header
    current_headerstart[1] = 1;                                 // sets use to 1 (allocated)
}

void merge_chunks(int* current_chunk, int* next_chunk) {        // takes int ptr arguments of current and next chunk
    current_chunk[0] = current_chunk[0] + next_chunk[0];        // sets the size of the first chunk to the first + second
    next_chunk[0] = 0;                                          // sets next chunk size to 0 
    next_chunk[1] = 0;                                          // sets use to 0 (deallocated)
}

void *mymalloc(size_t size, char* file, int line) {
    size = align(size);                                                                             // ensures allignment 
    if(size <= 0 || size > MEMLENGTH*sizeof(double)-sizeof(metadata)){                              // checks if size is bigger than 4096 bytes or less than or equal to 0 
        printf("Error at %s:%d: Invalid size\n", file, line);                                       // error message
        return NULL;
    }
    
    metadata chunk;                                                                                 // metadata struct to represent current chunk
    metadata *start_ptr = (metadata*)memory;                                                        // ptr to beginning of memory in terms of metadata
    metadata *payload_ptr = NULL;                                                                   // ptr returned to client if valid malloc conditions

    if(DEBUG) printf("you've gotten here alloc step 1");                                            // for debugging

    while((double*)start_ptr <= &memory[MEMLENGTH-(sizeof(metadata)/sizeof(double))]) {             // scans entire memory array in metadata increments
    if(DEBUG) printf("you've gotten here alloc step 2");                                            // for debugging
        int *curr_header = (int *)start_ptr;                                                        // creates a header at the beginning of memory as an int* to set metadata values
        chunk.size = curr_header[0];                                                                // initializes size as 0 (no size)
        chunk.use = curr_header[1];                                                                 // initializes in_use as 0 (not in use)

        if(chunk.size == 0 && chunk.use == 0) {                                                     // if size is 0 and in use is false
            if(DEBUG) printf("you've gotten here alloc step 3");                                    // for debugging
            assign_header(curr_header, size + sizeof(metadata));                                    // sets requested values to current header
            payload_ptr = start_ptr + 1;                                                            // payload will be one metadata space away from the current position being scanned, hence, start_ptr + 1
            init_next_chunk(curr_header, MEMLENGTH*(sizeof(double)) - (size + sizeof(metadata)));   // calls helper method to find and create next chunk, including space left and if in use (by default not)
            return (void *)payload_ptr;                                                             // returns the ptr to the payload to the client
        }
        if(chunk.size >= size + sizeof(metadata) && chunk.use == 0) {                               // checks if chunk has enough space and is free
            if(DEBUG) printf("you've gotten here alloc step 4");                                    // for debugging 
            assign_header(curr_header, size + sizeof(metadata));                                    // sets requested values to current header
            payload_ptr = start_ptr + 1;                                                            // payload will be one metadata space away from the current position being scanned, hence, start_ptr + 1
            if(chunk.size > (size + sizeof(metadata))) {                                            // checks if there will be free space afterwards
                if(DEBUG) printf("you've gotten here alloc step 5");                                // for debugging
                init_next_chunk(curr_header, chunk.size - (size + sizeof(metadata)));               // if requested size is small than available, finds location of next header
            }
            return (void *)payload_ptr;                                                             // returns ptr to the payload to the client
        }
        if(chunk.size < (size + sizeof(metadata)) || chunk.use != 0) {                              // if not enough space available or if chunk is already allocated
            if(DEBUG) printf("you've gotten here alloc step 6");                                    // for debugging
            start_ptr = find_next_chunk((int *)start_ptr);                                          // moves start_ptr to beginning of netx header
            if(start_ptr == NULL) {                                                                 // if this goes outside the arry it breaks out of the loop
                if(DEBUG) printf("you've gotten here alloc step 7");                                // for debugging
                break;
            }
        }
        if(DEBUG) printf("you've gotten here alloc step 8");                                        // for debugging
    }
    if(DEBUG) printf("you've gotten here step 9");                                                  // for debugging
    printf("Error at %s:%d: Not enough memory :(\n", file, line);                                   // prints if no space found for requested size. O(n)
    return NULL;
}


void myfree(void* ptr, char* file, int line) {
    if (ptr == NULL || ptr < (void*)memory || ptr >= (void*)&memory[MEMLENGTH]) {                   // checks for invalid cases
        printf("Error at %s:%d: Trying to free a null pointer\n", file, line);                      // error statement
        return;
    }
    char *start_ptr = (char *)memory;                                                               // casts pointers as char to read memory at byte level
    if(DEBUG) printf("address of start_ptr: %p\n", start_ptr);                                      // for debugging
    if(DEBUG) printf("you've gotten here step 1");                                                  // for debugging
    metadata chunk;                                                                                 

    while(start_ptr <= (char*)&memory[MEMLENGTH-(sizeof(metadata)/sizeof(double))]) {               // scans through array. same method as mymalloc()
        if(DEBUG) printf("you've gotten here step 2");                                              // debugging
        int *curr_header = (int *)start_ptr;                                                        // int* to change values within metadata 
        chunk.size = curr_header[0];                                                                // sets size to 0
        chunk.use = curr_header[1];                                                                 // sets use to 0 
        if(chunk.use == 0 && (start_ptr + chunk.size + sizeof(metadata)) == ptr) {                  // checks for if the data is allocated and if address is the same as pointer (mainly first instance)
            if(DEBUG) printf("you've gotten here step 3");                                          // for debugging
            int *curr_chunk = (int *)ptr - sizeof(metadata)/sizeof(int);                            // points to metadata of chunk being deallocated
            if(curr_chunk[1] == 0) {                                                                // checks if use is 0 (deallocated)
                if(DEBUG) printf("you've gotten here step 4");                                      // for debugging
                printf("Error at %s:%d: Freed this memory already :(\n", file, line);               // error statement
                return;                                                                             
            }
            int *next_chunk = find_next_chunk(curr_header);                                         // set nextChunk to point to the next chunk of currentChunk
            if(next_chunk != NULL && next_chunk[1] == 0) {                                          // checks to see if the current and previous chunks are in use
                merge_chunks((int *)start_ptr, next_chunk);                                         // coalesces free adjacent chunks
            }
            merge_chunks((int *)start_ptr, (int *)((char *)ptr - sizeof(metadata)));                // merge
            ptr = NULL;                                                                             // fully deallocates the pointer
            return;
        }

        if ((start_ptr + sizeof(metadata)) == (char *)ptr) {                                        // checks for if the data is allocated and if address is the same as pointer 
            if(DEBUG) printf("you've gotten here step 5");                                          // for debugging
            int *curr_chunk = (int *)ptr - sizeof(metadata)/sizeof(int);                            // points to metadata of chunk being deallocated
            if(curr_chunk[1] == 0) {
                if(DEBUG) printf("you've gotten here step 6");                                      // for debugging
                printf("Error at %s:%d: Freed this memory already :(\n", file, line); 
                return;
            }
            int *nextChunk = find_next_chunk(curr_chunk);                                           // looks for chunk to see if use is set to 0
            if(nextChunk != NULL && nextChunk[1] == 0) {
                if(DEBUG) printf("you've gotten here step 7");                                      // for debugging
                merge_chunks((int *)start_ptr, nextChunk);                                          // if yes, it merges
            }
            curr_chunk[1] = 0;
            ptr = NULL;
            return;
        }
        start_ptr = (char *)find_next_chunk((int *)start_ptr);                                      // next few lines checks for if the next chunk is null (end of memory 
        if(start_ptr == NULL) {                                                                     // or looped through all allocations)
            if(DEBUG) printf("you've gotten here step 8");                                          // for debugging
            break;
        }
        if(DEBUG) printf("you've gotten here step 9");                                              // for debugging
    }   
    if(DEBUG) printf("you've gotten here step 10");                                                 // for debugging
    printf("Error at %s:%d: This pointer was not initialized :(\n", file, line);                    // thank you for reading through our code
}

int memClean() {
    metadata chunk;
    metadata *start_ptr = (metadata *)memory;
    int *curr_header = (int *)start_ptr; 
    chunk.size = curr_header[0];
    chunk.use  = curr_header[1];

    if ((chunk.size == 0 && chunk.use == 0) || (chunk.size == MEMLENGTH * sizeof(metadata) && chunk.use == 0)) {
        return 1;
    }

    return 0;
}
