#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


#define MEMLENGTH 512
static double memory[MEMLENGTH];

typedef struct metadata{
    int size;                           // size of memory chunk (including header)
    int use;                            // flag to see if chunk is allocated (1 - allocated, 0 - not allocated)
    //struct metadata *next;            // pointer to the next available free space  (not going with this method because increases memory usage by a lot and not necessary for small data sizes)
} metadata;

size_t align(size_t size) {                 // method to align everything as 8-byte aligned
    return (size+7) & ~7;                   // uses addition and bitwise and to round up to nearest multiple of 8
}

void *find_next_chunk(int* current) {
    int* nextChunk = (int *)(current + current[0]/sizeof(int));
    if(nextChunk <= (int *)(&memory[MEMLENGTH - (sizeof(metadata)/sizeof(double))])) {
        return (void *)nextChunk;
    } 
    return NULL;
}

void init_next_chunk(int* curr_header, size_t size) {
    int* nextcurr_header = find_next_chunk(curr_header);
    if(nextcurr_header != NULL) {
        nextcurr_header[0] = size;
        nextcurr_header[1] = 0;
    }
}

void merge_chunks(int* current, int* next) {
    current[0] = current[0] + next[0];
    next[0] = 0;
    next[1] = 0;
}

void *mymalloc(size_t size, char* file, int line) {
    size = align(size);                                                         // ensures allignment 
    if(size <= 0 || size > MEMLENGTH*sizeof(double)-sizeof(metadata)){           // checks if size is bigger than 4096 bytes or less than or equal to 0 
        printf("Error at %s:%d: Invalid size\n", file, line);                   // error message
        return NULL;
    }
    
    metadata chunk;
    double *payload_ptr = NULL;
    double *start_ptr = memory;

    while(start_ptr <= &memory[MEMLENGTH-(sizeof(metadata)/sizeof(double))]) {
        int *curr_header = (int *)start_ptr;
        chunk.size = curr_header[0];
        chunk.use = curr_header[1];

        if(chunk.size == 0 && chunk.use == 0) {
            curr_header[0] = size + sizeof(metadata);
            curr_header[1] = 1;
            payload_ptr = start_ptr + 1;
            init_next_chunk(curr_header, MEMLENGTH*(sizeof(double)) - (size + sizeof(metadata)));
            return (void *)payload_ptr;
        }
        if(chunk.size >= size + sizeof(metadata) && chunk.use == 0) {
            curr_header[0] = size + sizeof(metadata);
            curr_header[1] = 1;
            payload_ptr = start_ptr + 1;
            if(chunk.size > (size + sizeof(metadata))) {
                init_next_chunk(curr_header, chunk.size - (size + sizeof(metadata)));
            }
            return (void *)payload_ptr;
        }
        if((chunk.size - (size + sizeof(metadata)) < 0) || chunk.use != 0) {
            start_ptr = find_next_chunk((int *)start_ptr);
            if(start_ptr == NULL) {
                break;
            }
        }

    }
    printf("Error at %s:%d: Not enough memory :(\n", file, line);
    return NULL;

}


void myfree(void* ptr, char* file, int line) {
    char *start_ptr = (char *)memory;
    metadata chunk;

    while(start_ptr <= (char*)&memory[MEMLENGTH-(sizeof(metadata)/sizeof(double))]) {
        int *curr_header = (int *)start_ptr;
        chunk.size = curr_header[0];
        chunk.use = curr_header[1];
        if(chunk.use == 0 && (start_ptr + chunk.size + sizeof(metadata)) == ptr) {
            int *curr_chunk = (int *)ptr - 8/sizeof(int);
            if(curr_chunk[1] == 0) {
                printf("Error: already freed memory\n");
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
            int *curr_chunk = (int *)ptr - sizeof(metadata)/sizeof(int);
            if(curr_chunk[1] == 0) {
                printf("Error: already freed memory\n");
                return;
            }
            int *nextChunk = find_next_chunk(curr_chunk);
            if(nextChunk != NULL && nextChunk[1] == 0) {
                merge_chunks((int *)start_ptr, nextChunk);
            }
            curr_chunk[1] = 0;
            ptr = NULL;
            return;
        }
        start_ptr = (char *)find_next_chunk((int *)start_ptr);
        if(start_ptr == NULL) {
            break;
        }
    }
    printf("Pointer not in heap\n");
}



