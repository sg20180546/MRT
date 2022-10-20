

all	: tetris.o list.o
	gcc -g tetris.c -lncurses

list.o:
	gcc -c list.c

clean:
	rm a.out *.o
