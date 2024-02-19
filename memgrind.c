#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.h"
#ifndef DEBUG
#define DEBUG 1
#endif

void test1() {
    void *memgrind_arr[4096];                                           // a test array used to store pointers to allocated memory
    for (int i = 0; i < 120; i++) {                                      // will repeat 120 times
        memgrind_arr[i] = malloc(1);                                    // allocating malloc at a position in memgrind_arr for testing
        if (DEBUG) {
            printf("memory allocated at %p\n", &memgrind_arr[i]);       // printing malloc statement
        } 
        free(memgrind_arr[i]);                                          // freeing allocated memory
        if (DEBUG) {
            printf("memory freed at %p\n", &memgrind_arr[i]);           // printing free statement
        }                       
    }
    memgrind_arr[18] = malloc(4050);                                    // should be able to be allocated 
}

void test2() {
    void *memgrind_arr[4096];
    for (int i = 0; i < 120; i++) {
        memgrind_arr[i] = malloc(1);
        if (DEBUG) {
            printf("memory allocated at %p\n", &memgrind_arr[i]);       // printing malloc statement
        } 
    }
    for (int i = 0; i < 120; i++) {
        free(memgrind_arr[i]);
        if (DEBUG) {
            printf("memory freed at %p\n", &memgrind_arr[i]);           // printing free statement
        } 
    }
    memgrind_arr[18] = malloc(4050);                                    // should be able to be allocated 
}

void test3() {
    void *memgrind_arr[120];



}

void test4() {
    void *memgrind_arr[4096];
    memgrind_arr[0] = malloc(4099);                                     // trys to allocate a memory that's too large, return error 
    free(memgrind_arr[0]);                                              // trys to free, should return error as it was not put into memory
}


int main(int argc, char* argv[]) {
    for (int i = 0; i < 50; i++) {
        test1();
        test2();
        test3();
    }
}