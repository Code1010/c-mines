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

int play_game(char ** board, char ** view, int size, int num);

void show_board(char ** view, int size, int cursr, int cursc);

int uncover(char board);

void print_summary(int num, int right, int size);

void clear_area(int r, int c, char ** view, char ** board);
#endif
