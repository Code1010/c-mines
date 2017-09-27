# compile c-mines by Cody Burrows

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 

mines: c-mines.c c-mines.h
	$(CC) c-mines.c -lncurses -o mines
