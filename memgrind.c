#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.h"

void test1() {
    for(int i = 0; i < 120; i++) {              // will repeat 120 times
        char *ptr = malloc(1);                  // allocating 1-byte 
        free(ptr);                              // freeing memory
    }
    printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
    }