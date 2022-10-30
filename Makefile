
OBEJCT= tetris.o rank.o list.o

.PHONY: a.out
a.out: tetris.o rank.o list.o
	gcc -g -o $@ $(OBEJCT) -lncurses

list.o:
	gcc -c list.c

rank.o:
	gcc -c rank.c

tetris.o:
	gcc -c tetris.c

clean:
	rm a.out *.o

