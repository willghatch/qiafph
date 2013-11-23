
CC=gcc
CFLAGS=-std=gnu99 -c


all: main.o notes.o
	gcc -std=gnu99 -o mgen main.o notes.o

clean:
	rm mgen *.o


