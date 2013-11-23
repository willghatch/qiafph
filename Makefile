
CC=gcc
CFLAGS=-std=gnu99 -c


all: main.o notes.o
	gcc -std=gnu99 -o mgen main.o notes.o

clean:
	rm mgen *.o

main.o: main.c
	$(CC) $(CFLAGS) main.c

notes.o: notes.c notes.h
	$(CC) $(CFLAGS) notes.c

