// c-mines.h
// @author Cody Burrows
// ///////////////////////////////////////

#ifndef CMINES
#define CMINES

void print_usage(void);

void init_board(char ** board, int size, int num);

char ** create_board(int size, char fill);

void destroy_board(char ** board, int size);

void print_board(char ** board, int size);

void calculate_cells(char ** board, int size);

#endif
