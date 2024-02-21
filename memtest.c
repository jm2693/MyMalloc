#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.h"
#ifndef DEBUG
#define DEBUG 1
#endif

//memtest.c example defintions
#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)

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
    
}

void testB() {
    void *memgrind_arr[2];
    memgrind_arr[0] = malloc(4099);                                     // trys to allocate a memory that's too large, return error 
    free(memgrind_arr[0]);                                              // trys to free, should return error as it was not put into memory
}

void testC(){
    void* ptr = malloc(0);
    free(ptr);                                      // trys to free, should return error as it was not put into memory
    
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
    
}

void testE() {
    int *p = malloc(sizeof(int)*2);
    free(p + 1);
    
}

void testF() {
    int *p = malloc(sizeof(int)*100);
    int *q = p;
    free(p);
    free(q);
    
}

void testG() {
    int x;
    free(&x);
}

void testH() {
    void *memtest_arr[2];
    memtest_arr[0] = malloc(-1);
    free(memtest_arr[0]);
}



int _main(int argc, char* argv[]) {

    testA();
    testB();
    testC();
    testD();
    testE();
    testF();
    testG();
    testH();

    //memtest.c example
    char *obj[OBJECTS];
	int i, j, errors = 0;
	
	// fill memory with objects
	for (i = 0; i < OBJECTS; i++) {
		obj[i] = malloc(OBJSIZE);
	}
	
	// fill each object with distinct bytes
	for (i = 0; i < OBJECTS; i++) {
		memset(obj[i], i, OBJSIZE);
	}
	
	// check that all objects contain the correct bytes
	for (i = 0; i < OBJECTS; i++) {
		for (j = 0; j < OBJSIZE; j++) {
			if (obj[i][j] != i) {
				errors++;
				printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
			}
		}
	}
	printf("%d incorrect bytes\n", errors);
	
	return EXIT_SUCCESS;

    return 0;
}