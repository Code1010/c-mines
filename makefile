# compile c-mines by Cody Burrows

CC=gcc
CDEVFLAGS=-Wall -Wextra -pedantic -std=c99 
CFLAGS=-o mines -lncurses

mines: c-mines.o
	$(CC) c-mines.o $(CFLAGS)

c-mines.o: c-mines.c
	$(CC) -c c-mines.c

dev: dev_obj
	$(CC) c-mines.o $(CFLAGS)

dev_obj: c-mines.c
	$(CC) -c c-mines.c $(CDEVFLAGS)

clean:
	-rm c-mines.o

real_clean: clean
	-rm mines


