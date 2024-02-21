#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.h"
#ifndef DEBUG
#define DEBUG 1
#endif

void test1() {
    void *memgrind_arr[120];                                            // a test array used to store pointers to allocated memory
    for (int i = 0; i < 120; i++) {                                     // will repeat 120 times
        memgrind_arr[i] = malloc(1);                                    // allocating malloc at a position in memgrind_arr for testing
        if (DEBUG) {
            printf("memory allocated at %p\n", memgrind_arr[i]);        // printing malloc statement
        } 
        free(memgrind_arr[i]);                                          // freeing allocated memory
        if (DEBUG) {
            printf("memory freed at %p\n", memgrind_arr[i]);            // printing free statement
        }                       
    }
    memgrind_arr[18] = malloc(1000);                                    // should be able to be allocated 
    printf("memory allocated at %p\n", memgrind_arr[18]);
    free(memgrind_arr[18]);
    printf("memory freed at %p\n", memgrind_arr[18]);                   // printing free statement
}

void test2() {
    void *memgrind_arr[120];
    for (int i = 0; i < 120; i++) {
        memgrind_arr[i] = malloc(1);
        if (DEBUG) {
            printf("memory allocated at %p\n", memgrind_arr[i]);       // printing malloc statement
        } 
    }

    for (int i = 0; i < 120; i++) {
        free(memgrind_arr[i]);
        if (DEBUG) {
            printf("memory freed at %p\n", memgrind_arr[i]);           // printing free statement
        } 
    } 
}


void test3() {
    char *memgrind_arr[120];                // array to store 120 pointers
    int allocated[120] = {0};               // initialize the memory allocation array
    int allocated_count = 0;                // both index and count

    while (allocated_count < 120) {
        if(allocated_count == 0 || (rand() % 2 == 0 && allocated_count < 120)) {    // allocate 1 byte of memory and store the address if rand says 0
            printf("memory allocated at index=%d: pointer to memory at %p\n", allocated_count, memgrind_arr[allocated_count]);
            memgrind_arr[allocated_count] = malloc(1);
            allocated[allocated_count] = 1;                                         // is allocated
            allocated_count++;                                                      // increase by 1
        } else {                                                                    // free the memory if rand says 1
            allocated_count--;                                                      // decrease by 1
            printf("memory freed at index=%d: pointer to memory at %p\n", allocated_count, memgrind_arr[allocated_count]);
            free(memgrind_arr[allocated_count]);                                    
            allocated[allocated_count] = 0;                                         // not allocated
        }
    }
}



int main(int argc, char* argv[]) {

    struct timeval stop, start;
    gettimeofday(&start, NULL);
    test1();
    gettimeofday(&stop, NULL);
    printf("test 1 took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
        
    gettimeofday(&start, NULL);
    test2();
    gettimeofday(&stop, NULL);
    printf("test 2 took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    test3();
    gettimeofday(&stop, NULL);
    printf("test 3 took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

    return 0;
}