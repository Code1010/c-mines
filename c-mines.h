// c-mines.h
// @author Cody Burrows
// ///////////////////////////////////////

#ifndef CMINES
#define CMINES

void print_usage(void);

char ** init_board(int size, int num);

void destroy_board(char ** board, int size);

void print_board(char ** board, int size);
#endif
