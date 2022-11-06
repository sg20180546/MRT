OBEJCT= tetris.o rank.o list.o
CFLAGS=-g -c
CC=gcc

.PHONY: a.out tetris.o
a.out: tetris.o rank.o list.o
	$(CC) -g -o $@ $(OBEJCT) -lncurses -lpthread

list.o:
	$(CC) $(CFLAGS) list.c

rank.o: 
	$(CC) $(CFLAGS) rank.c 

tetris.o:
	$(CC) $(CFLAGS) tetris.c

clean:
	rm a.out *.o

