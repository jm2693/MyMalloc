#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
//#include "mymalloc.h"
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
    for (int i = 0; i < 120; i++) {                                    // allocates 120 1-byte chunks
        memgrind_arr[i] = malloc(1);
        if (DEBUG) {
            printf("memory allocated at %p\n", memgrind_arr[i]);       // printing malloc statement
        } 
    }

    for (int i = 0; i < 120; i++) {                                    // frees ptr at each 1-byte chunk
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

    for(int i = 0; i < 120; i++) {
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
    for (int i = 0; i < 120; i++) {
        if (allocated[i] == 1){
        free(memgrind_arr[i]);
        }
    }
}

void test4(){
    for (int i = 0; i < 120; i++) {
    char* ptr1 = (char*)malloc(16);                     // allocating 16 byte chunk (total 24)
    char* ptr2 = (char*)malloc(16);                     // allocating 16 byte chunk (total 24)
    char* ptr3 = (char*)malloc(16);                     // allocating 16 byte chunk (total 24)

    free(ptr2);                                         // freeing 2nd chunk
    free(ptr1);                                         // freeing 1st. Should coalece 

    char* ptr4 = (char*)malloc(32);                     // should allocate before ptr3

    if(ptr4 != NULL && ptr4 >= ptr1 && ptr4 < ptr3) printf("allocated in correct spot ");
    else printf("failed to allocate correctly ");

    free(ptr3);                                         // frees remaining
    free(ptr4);                                         // frees remaining
    }

}


void test5(){
    void *memgrind_arr[10];
    for(int i = 0; i < 4; i++){
        memgrind_arr[i] = malloc(1000);
        if (DEBUG) {
            printf("memory allocated at %p\n", memgrind_arr[i]);       // printing malloc statement
        } 
    }
    free(memgrind_arr[0]);
    if (DEBUG) {
            printf("memory freed at %p\n", memgrind_arr[0]);       // printing malloc statement
        } 
    free(memgrind_arr[2]);
    if (DEBUG) {
            printf("memory freed at %p\n", memgrind_arr[2]);       // printing malloc statement
        } 
    memgrind_arr[3] = malloc(2000);                                     // should fail
}

int main(int argc, char* argv[]) {

    struct timeval stop, start;
    double elapsed_time_ms;
    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) {                              // running each test 50 times

    test1();
    
    }
    gettimeofday(&stop, NULL);
    elapsed_time_ms = (stop.tv_sec - start.tv_sec) * 1000.0; // seconds to milliseconds
    elapsed_time_ms += (stop.tv_usec - start.tv_usec) / 1000.0; // microseconds to milliseconds
    printf("test 1 took %.2f ms\n", elapsed_time_ms/50); 
    elapsed_time_ms = 0;

    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) {

    test2();
    
    }
    gettimeofday(&stop, NULL);
    elapsed_time_ms = (stop.tv_sec - start.tv_sec) * 1000.0; // seconds to milliseconds
    elapsed_time_ms += (stop.tv_usec - start.tv_usec) / 1000.0; // microseconds to milliseconds
    printf("test 2 took %.2f ms\n", elapsed_time_ms/50); 
    elapsed_time_ms = 0;

    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) {

    test3();
    
    }
    gettimeofday(&stop, NULL);
    elapsed_time_ms = (stop.tv_sec - start.tv_sec) * 1000.0; // seconds to milliseconds
    elapsed_time_ms += (stop.tv_usec - start.tv_usec) / 1000.0; // microseconds to milliseconds
    printf("test 3 took %.2f ms\n", elapsed_time_ms/50); 
    elapsed_time_ms = 0;


    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) {

    test4();
    }
    gettimeofday(&stop, NULL);
    elapsed_time_ms = (stop.tv_sec - start.tv_sec) * 1000.0; // seconds to milliseconds
    elapsed_time_ms += (stop.tv_usec - start.tv_usec) / 1000.0; // microseconds to milliseconds
    printf("test 4 took %.2f ms\n", elapsed_time_ms/50); 
    elapsed_time_ms = 0;

    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) {

    test5();

    }
    gettimeofday(&stop, NULL);
    elapsed_time_ms = (stop.tv_sec - start.tv_sec) * 1000.0; // seconds to milliseconds
    elapsed_time_ms += (stop.tv_usec - start.tv_usec) / 1000.0; // microseconds to milliseconds
    printf("test 5 took %.2f ms\n", elapsed_time_ms/50); 
    elapsed_time_ms = 0;

    return 0;
}