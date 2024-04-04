CC=gcc
CFLAGS=-Wall -fsanitize=address,leak -g -lpthread

all:
	mkdir -p build
	$(CC) $(CFLAGS) -o build/main main.c

clean:
	rm build/main

run:
	./build/main

