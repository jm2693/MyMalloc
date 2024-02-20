#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.h"
#ifndef DEBUG
#define DEBUG 1
#endif



void test1() {                                          // what happens if you malloc to the same location twice? and try to free it twice? should fail on both counts
    void* ptr = malloc(1);                              // mallocs 1 byte (really 8) to ptr
    printf("memory allocated at %p\n", ptr); 
    ptr = malloc(3);                                    // mallocs 3 bytes (really 8) also to ptr
    printf("memory allocated at %p\n", ptr); 
    free(ptr);                                          // tries to free twice
    if (DEBUG) {
        printf("memory freed at %p\n", ptr);            // printing free statement
    } 
    free(ptr);
    if (DEBUG) {
        printf("memory freed at %p\n", ptr);            // printing free statement
    } 
}


void test2() {                                          // trying to allocate more memory than possible. should fail
    void *memgrind_arr[2];
    memgrind_arr[0] = malloc(4099);                     // trys to allocate a memory that's too large, return error 
    free(memgrind_arr[0]);                              // trys to free, should return error as it was not put into memory
}


void test3(){
    void *memgrind_arr[10];
    memgrind_arr[0] = malloc(0);                                        // trys to allocate a memory that's too large, return error 
    free(memgrind_arr[0]);                                              // trys to free, should return error as it was not put into memory
}


void test4(){
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


void test5() {                          // tries to free something that was not allocated. should return error
    void* ptr;
    free(ptr);
}


int main(int argc, char* argv[]) {

    test1();
    test2();
    test3();
    test4();
    test5();

    // for (int i = 0; i < 50; i++) {
    //     // test6();
    //     //test3();
    // }
    // char* arr[5];
    // arr[1] = malloc(1);
    // printf("mem allocated at %p\n", arr[1]);
    // arr[2] = malloc(2);
    // printf("mem allocated at %p\n", arr[2]);
    // free(arr[1]);
    // printf("mem deallocated at %p\n", arr[1]);
    // free(arr[2]);
    // printf("mem deallocated at %p\n", arr[2]);
}