// c-mines.c
// @author Cody Burrows
// /////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include "c-mines.h"

int main(int argc, char ** argv) {

    if(argc == 3) {
        
        int size, num;
        size = strtol(argv[1], NULL, 10);
        num = strtol(argv[2], NULL, 10);

        if(size <= 0 || num <= 0) {
            print_usage();
            return EXIT_FAILURE;
        } else if(size >= 1024) {
            size = 1024;
        } else if(num >= (size * size)) {
            num = (size * size);
        }

        printf("================C-MINES================\n");
        printf("    == Board Size: %dx%d\n", size, size);
        printf("    == Number of mines: %d\n", num);
        printf("=======================================\n");

        char ** board = init_board(size, num);

        destroy_board(board, size);
        return EXIT_SUCCESS;
        
    } else {
        print_usage();
        return EXIT_FAILURE;
    }
}

void print_usage(void) {

    fprintf(stderr, "Usage: mines len num\n");
    fprintf(stderr, "  where len is the decimal length of one side of the board\n");
    fprintf(stderr, "  and   num is the number of mines on the board\n");
    
}

char ** init_board(int size, int num) {

    char ** board = malloc(size);

    for(int i = 0; i < size; ++i) {

        board[i] = calloc(1, size);
        for(int j = 0; j < size; ++j) {

            board[i][j] = '#';

        }

    }

    return board;
}

void destroy_board(char ** board, int size) {

    for(int i = 0; i < size; ++i) {

        free(board[i]);

    }

    free(board);

}
