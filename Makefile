CC = gcc -Wall -Wvla -std=c99 -fsanitize=address
CFLAGS=-I.
DEPS = mymalloc.h
OBJ = memgrind.o memtest.o mymalloc.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

memgrind: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
