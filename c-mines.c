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

        print_board(board, size);

        destroy_board(board, size);
        return EXIT_SUCCESS;
        
    } else {
        print_usage();
        return EXIT_FAILURE;
    }
}

void print_usage(void) {

    fprintf(stderr, "Usage: mines len num\n");
    fprintf(stderr, "  where len is the integer length of one side of the board\n");
    fprintf(stderr, "  and   num is the number of mines on the board\n");
    
}

/*
 * Legend:
 * #   = covered
 * *   = mine
 * .   = empty
 * 1-8 = number of adjacent mines
 */
char ** init_board(int size, int num) {

    char ** board = malloc(size * sizeof(char *));

    for(int i = 0; i < size; ++i) {

        board[i] = malloc(size * sizeof(char *));
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

void print_board(char ** board, int size) {
    for(int r = 0; r < size; ++r) {
        for(int c = 0; c < size; ++c) {
            printf(" %c ", board[r][c]);
        }

        printf("\n");
    }
}
