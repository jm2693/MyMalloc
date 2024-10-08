CC = gcc -Wall -Wvla -std=c99 -fsanitize=address
CFLAGS=-I.
DEPS = mymalloc.h
OBJ = memgrind.o mymalloc.o memtest.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

memgrind: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
