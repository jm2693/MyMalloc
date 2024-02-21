#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.h"
#ifndef DEBUG
#define DEBUG 1
#endif




void testA() {
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
    // memClean();
}

void testB() {
    void *memgrind_arr[2];
    memgrind_arr[0] = malloc(4099);                                     // trys to allocate a memory that's too large, return error 
    free(memgrind_arr[0]);                                              // trys to free, should return error as it was not put into memory
    // memClean();
}

void testC(){
    void* ptr = malloc(0);
    free(ptr);                                      // trys to free, should return error as it was not put into memory
    // memClean();
}

void testD(){
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
    // memClean();
}

void testE() {
    int *p = malloc(sizeof(int)*2);
    free(p + 1);
    // memClean();
}

void testF() {
    int *p = malloc(sizeof(int)*100);
    int *q = p;
    free(p);
    free(q);
    // memClean();
}

void testG() {
    int x;
    free(&x);
    // memClean();
}



int main(int argc, char* argv[]) {

    // testA();
    // testB();
    // testC();
    // testD();
    // testE();
    // testA();
    // testB();
    // testC();
    // testD();
    // testE();
    // testF();
     testG();

    // for (int i = 0; i < 50; i++) {
    //     // test6();
    //     //test3();
    // }
    return 0;
}