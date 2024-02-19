CC = gcc -Wall -Wvla -std=c99 -fsanitize=address
DEPS = mymalloc.h
OBJECTS = mymalloc.o memgrind.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

memgrind: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
    rm -f $(OBJ) memgrind
	