CC = gcc -Wall -Wvla -std=c99 -fsanitize=address
RM = rm -f 
MEMGRIND = memgrind 
SRCS = mymalloc.c memgrind.c
OBJECTS = $(SRCS:.c=.o)


