
CC=gcc
DEBUGFLAGS=-g
OPTIMIZEFLAGS=-O2
CFLAGS=-std=gnu99 -c $(OPTIMIZEFLAGS)
OBJECTS=main.o notes.o chordinfo.o globalvars.o score.o mutations.o md_out.o midge_out.o analysis_out.o
EXECUTABLE=qiafph


all: $(OBJECTS) rulescores.h
	$(CC) -o $(EXECUTABLE) $(OBJECTS)

score.o: score.c score.h rulescores.h
	$(CC) $(CFLAGS) score.c

clean:
	rm $(EXECUTABLE) *.o


