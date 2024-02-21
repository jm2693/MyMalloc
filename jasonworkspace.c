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
    //struct metadata *next;            // pointer to the next available free space  (not going with this method because increases memory usage by a lot and not necessary for small data sizes)
} metadata;

size_t align(size_t size) {             // method to align everything as 8-byte aligned
    return (size+7) & ~7;               // uses addition and bitwise and to round up to nearest multiple of 8
}

void *find_next_chunk(int* current) {                                                       // takes current beginning of chunk and 
    int* nextChunk = (int *)(current + current[0]/sizeof(int));
    if(nextChunk <= (int *)(&memory[MEMLENGTH - (sizeof(metadata)/sizeof(double))])) {
        return (void *)nextChunk;
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
    current_headerstart[0] = size;                              // 
    current_headerstart[1] = 1;
}

void merge_chunks(int* current_chunk, int* next_chunk) {
    current_chunk[0] = current_chunk[0] + next_chunk[0];
    next_chunk[0] = 0;
    next_chunk[1] = 0;
}

void *mymalloc(size_t size, char* file, int line) {
    size = align(size);                                                         // ensures allignment 
    if(size <= 0 || size > MEMLENGTH*sizeof(double)-sizeof(metadata)){          // checks if size is bigger than 4096 bytes or less than or equal to 0 
        printf("Error at %s:%d: Invalid size\n", file, line);                   // error message
        return NULL;
    }
    
    metadata chunk;
    metadata *start_ptr = (metadata*)memory;
    metadata *payload_ptr = NULL;

    if(DEBUG) printf("you've gotten here alloc step 1");

    while((double*)start_ptr <= &memory[MEMLENGTH-(sizeof(metadata)/sizeof(double))]) {
    if(DEBUG) printf("you've gotten here alloc step 2");
        int *curr_header = (int *)start_ptr;
        chunk.size = curr_header[0];
        chunk.use = curr_header[1];

        if(chunk.size == 0 && chunk.use == 0) {
            if(DEBUG) printf("you've gotten here alloc step 3");
            assign_header(curr_header, size + sizeof(metadata));
            payload_ptr = start_ptr + 1;
            init_next_chunk(curr_header, MEMLENGTH*(sizeof(double)) - (size + sizeof(metadata)));
            return (void *)payload_ptr;
        }
        if(chunk.size >= size + sizeof(metadata) && chunk.use == 0) {
            if(DEBUG) printf("you've gotten here alloc step 4");
            assign_header(curr_header, size + sizeof(metadata));
            payload_ptr = start_ptr + 1;
            if(chunk.size > (size + sizeof(metadata))) {
                if(DEBUG) printf("you've gotten here alloc step 5");
                init_next_chunk(curr_header, chunk.size - (size + sizeof(metadata)));
            }
            return (void *)payload_ptr;
        }
        if(chunk.size < (size + sizeof(metadata)) || chunk.use != 0) {
            if(DEBUG) printf("you've gotten here alloc step 6");
            start_ptr = find_next_chunk((int *)start_ptr);
            if(start_ptr == NULL) {
                if(DEBUG) printf("you've gotten here alloc step 7");
                break;
            }
        }
        if(DEBUG) printf("you've gotten here alloc step 8");

    }
    if(DEBUG) printf("you've gotten here step 9");
    printf("Error at %s:%d: Not enough memory :(\n", file, line);
    return NULL;

}


void myfree(void* ptr, char* file, int line) {
    if (ptr == NULL) {
        printf("Error at %s:%d: Trying to free a null pointer\n", file, line);
        return;
    }
    char *start_ptr = (char *)memory;
    if(DEBUG) printf("what is this %p\n", start_ptr);
    if(DEBUG) printf("you've gotten here step 1");
    metadata chunk;

    while(start_ptr <= (char*)&memory[MEMLENGTH-(sizeof(metadata)/sizeof(double))]) {
        if(DEBUG) printf("you've gotten here step 2");
        int *curr_header = (int *)start_ptr;
        chunk.size = curr_header[0];
        chunk.use = curr_header[1];
        if(chunk.use == 0 && (start_ptr + chunk.size + sizeof(metadata)) == ptr) {
            if(DEBUG) printf("you've gotten here step 3");
            int *curr_chunk = (int *)ptr - 8/sizeof(int);
            if(curr_chunk[1] == 0) {
                if(DEBUG) printf("you've gotten here step 4");
                printf("Error at %s:%d: Freed this memory already :(\n", file, line);  
                return;
            }
            int *next_chunk = find_next_chunk(curr_header);
            if(next_chunk != NULL && next_chunk[1] == 0) {
                merge_chunks((int *)start_ptr, next_chunk);
            }
            merge_chunks((int *)start_ptr, (int *)((char *)ptr - sizeof(metadata)));
            ptr = NULL;
            return;
        }

        if ((start_ptr + sizeof(metadata)) == (char *)ptr) {
            if(DEBUG) printf("you've gotten here step 5");
            int *curr_chunk = (int *)ptr - sizeof(metadata)/sizeof(int);
            if(curr_chunk[1] == 0) {
                if(DEBUG) printf("you've gotten here step 6");
                printf("Error at %s:%d: Freed this memory already :(\n", file, line); 
                return;
            }
            int *nextChunk = find_next_chunk(curr_chunk);
            if(nextChunk != NULL && nextChunk[1] == 0) {
                if(DEBUG) printf("you've gotten here step 7");
                merge_chunks((int *)start_ptr, nextChunk);
            }
            curr_chunk[1] = 0;
            ptr = NULL;
            return;
        }
        start_ptr = (char *)find_next_chunk((int *)start_ptr);
        if(start_ptr == NULL) {
            if(DEBUG) printf("you've gotten here step 8");
            break;
        }
        if(DEBUG) printf("you've gotten here step 9");
    }
    if(DEBUG) printf("you've gotten here step 10");
    printf("Error at %s:%d: This pointer was not initialized :(\n", file, line);
}


