#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.h"
#ifndef DEBUG
#define DEBUG 1
#endif

void test1() {
    void *memgrind_arr[120];                                           // a test array used to store pointers to allocated memory
    for (int i = 0; i < 120; i++) {                                      // will repeat 120 times
        memgrind_arr[i] = malloc(1);                                    // allocating malloc at a position in memgrind_arr for testing
        if (DEBUG) {
            printf("memory allocated at %p\n", memgrind_arr[i]);       // printing malloc statement
        } 
        free(memgrind_arr[i]);                                          // freeing allocated memory
        if (DEBUG) {
            printf("memory freed at %p\n", memgrind_arr[i]);           // printing free statement
        }                       
    }
    memgrind_arr[18] = malloc(1000);                                    // should be able to be allocated 
    printf("memory allocated at %p\n", memgrind_arr[18]);
    free(memgrind_arr[18]);
    printf("memory freed at %p\n", memgrind_arr[18]);           // printing free statement
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
    void* memgrind_arr[120];
        int allocated_count = 0;
        srand(time(NULL)); // Seed for random number generator

        while (allocated_count < 120) {
            int choice = rand() % 2; // Random choice between 0 and 1

            if (choice == 0) {
                // Allocate a 1-byte object and add the pointer to the array
                memgrind_arr[allocated_count] = malloc(1);
                if (memgrind_arr[allocated_count] == NULL) {
                    fprintf(stderr, "memory allocation failed\n");
                    break;
                }
                printf("memory allocated at index %d: pointer to address %p\n", allocated_count, memgrind_arr[allocated_count]);
                allocated_count++;
            } else if (choice == 1 && allocated_count > 0) {
                // Deallocate a previously allocated object
                free(memgrind_arr[allocated_count - 1]);
                memgrind_arr[allocated_count - 1] = NULL;
                printf("memory deallocated at index %d: pointer to address %p\n", allocated_count - 1, memgrind_arr[allocated_count-1]);
                allocated_count--;
            }
        }

        // Deallocate any remaining objects
        for (int i = 0; i < allocated_count; ++i) {
            free(memgrind_arr[i]);
            printf("memory deallocated at index %d: pointer to address %p\n", i, memgrind_arr[allocated_count]);
        }
    }




void test4() {
    void* ptr = malloc(1);
    printf("memory allocated at %p\n", ptr); 
    ptr = malloc(3);
    printf("memory allocated at %p\n", ptr); 
    free(ptr);
    if (DEBUG) {
        printf("memory freed at %p\n", ptr);           // printing free statement
    } 
    free(ptr);
    if (DEBUG) {
        printf("memory freed at %p\n", ptr);           // printing free statement
    } 
}

void test5() {
    void *memgrind_arr[2];
    memgrind_arr[0] = malloc(4099);                                     // trys to allocate a memory that's too large, return error 
    free(memgrind_arr[0]);                                              // trys to free, should return error as it was not put into memory
}

void test6(){
    void *memgrind_arr[10];
    memgrind_arr[0] = malloc(0);                                        // trys to allocate a memory that's too large, return error 
    free(memgrind_arr[0]);                                              // trys to free, should return error as it was not put into memory
}

void test7(){
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

void test8() {
    void* ptr;
    free(ptr);
}

int main(int argc, char* argv[]) {

    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test6();
    //test7();
    //test8();

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