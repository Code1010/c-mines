// c-mines.c
// @author Cody Burrows
// /////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <stdbool.h>
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
/*
        printf("================C-MINES================\n");
        printf("    == Board Size: %dx%d\n", size, size);
        printf("    == Number of mines: %d\n", num);
        printf("    == Difficulty: %d\n", (10 * num) / (size * size));
        printf("=======================================\n");
*/
        char ** board = create_board(size, '0');
        init_board(board, size, num);

        char ** view = create_board(size, '#');

        play_game(board, view, size, num);

        destroy_board(view, size);
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

// just allocates a board of specified size!
char ** create_board(int size, char fill) {

    char ** board = malloc(size * sizeof(char *));

    for(int i = 0; i < size; ++i) {

        board[i] = malloc(size * sizeof(char *));
        for(int j = 0; j < size; ++j) {

            board[i][j] = fill;

        }

    }

    return board;

}

/*
 * Legend:
 * #   = covered
 * *   = mine
 * 0   = empty
 * 1-8 = number of adjacent mines
 */
void init_board(char ** board, int size, int num) {


    int count = 0;
    int r, c;

    srand(time(0));
    while(count < num) {
        r = rand() % size;
        c = rand() % size;

        if(board[r][c] != '*') {
            count++;
            board[r][c] = '*';
        }
    }

    calculate_cells(board, size);

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

void calculate_cells(char ** board, int size) {

    for(int r = 0; r < size; ++r) {
        for(int c = 0; c < size; ++c) {

            if(board[r][c] != '*') {
                int total = 0;
                for(int sr = r - 1; sr <= r + 1; ++sr) {
                    for(int sc = c - 1; sc <= c + 1; ++sc) {

                        // make sure they're safe to dereference
                        if((sr >= 0 && sc >= 0) && (sr < size) && (sc < size)) {
                            if(board[sr][sc] == '*')
                                total++;
                        }
                    }
                }

                board[r][c] = total + '0';

            }

        }
    }
}

/*
 * Controls: 
 * Move cursor: wasd
 * Mark: e
 * Uncover cell: r
 * Quit: q
 */
int play_game(char ** board, char ** view, int size, int num) {
    
    time_t start = time(NULL);
    initscr();
    raw();
    noecho();
    curs_set(0);

    // meat goes here
    int mines_marked = 0;
    int r = 0, c = 0;
    bool safe = true;
    int ch;
    
    while(safe) {
        
        show_board(view, size, r, c);

        ch = getch();
        if(ch == 'q') {
            // i give up
            safe = false;
        } else if(ch == 'w') {
            r--;
        } else if(ch == 's') {
            r++;
        } else if(ch == 'a') {
            c--;
        } else if(ch == 'd') {
            c++; // lol
        }

        if(r < 0)
            r = size - 1;

        if(c < 0)
            c = size - 1;

        r = r % size;
        c = c % size;

    }

    refresh();
    getch();
    endwin();
    time_t end = time(NULL);
    return end - start;

}

void show_board(char ** view, int size, int cursr, int cursc) {
    
    move(0, 0);

    for(int r = 0; r < size; ++r) {
        for(int c = 0; c < size; ++c) {
            if(cursr == r && cursc == c) {
                printw("[%c]", view[r][c]);
            } else {
                printw(" %c ", view[r][c]);
            }
        }

        printw("\n");
    }

    refresh();

}
