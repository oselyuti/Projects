CC = g++
CFLAGS = -g -Wall
BIN = Mazemake Mazesolve

all: $(BIN)

Mazemake: Mazemake.cpp dset.h
	$(CC) $(CFLAGS) -o $@ $^

Mazesolve: Mazesolve.cpp
	$(CC) $(CFLAGS) -o $@ $<
#.cpp:
#	$(CC) $(CFLAGS) -o $* $^


clean: 
	rm -f Mazemake
	rm -f Mazesolve
