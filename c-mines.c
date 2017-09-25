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

        board[i] = malloc(size * sizeof(char));
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
    bool run = true;
    bool dead = false;
    int ch;
    
    while(run) {
        
        show_board(view, size, r, c);

        ch = getch();
        if(ch == 'q') {
            // i give up
            run = false;
            dead = true;
        } else if(ch == 'w') {
            r--;
        } else if(ch == 's') {
            r++;
        } else if(ch == 'a') {
            c--;
        } else if(ch == 'd') {
            c++; // lol
        } else if(ch == 'e') {
            // mark the space
            if(view[r][c] == '!') {
                view[r][c] = '#';
                
                if(board[r][c] == '*'){
                    mines_marked--;
                } 
            } else if(view[r][c] == '#') {
                view[r][c] = '!';

                if(board[r][c] == '*') {
                    mines_marked++;
                }
            }
        } else if(ch == 'r') {
            // detonate
            switch(uncover(board[r][c])) {
                case 0:
                    view[r][c] = board[r][c];
                    break;

                case 1:
                    view[r][c] = '*';
                    // you lose!
                    run = false;
                    dead = true;
                    break;

                case -1:
                    view[r][c] = '.';
                    Cell * head = NULL;
                    Cell ** handle = &head;
                    clear_area(r, c, view, board, handle, size);
                    destroy_cells(handle);
                    break;
            }
            
        }

        if(r < 0)
            r = size - 1;

        if(c < 0)
            c = size - 1;

        r = r % size;
        c = c % size;

        if(mines_marked == num) {
            // win!
            run = false;
            dead = false;
        } 

    }

    time_t end = time(NULL);
    refresh();
    curs_set(1);
    endwin();

    // game over...did you win?
    if(dead) {
        printf("\n\nYou died in a glorious ball of fire after only"
                " %ld seconds.\n\n", (long) end - (long) start);
        print_summary(num, mines_marked, size);
    } else {
        printf("\n\nNice work! You found all of the mines in %ld seconds. \n\n",
                (long) end - (long) start);
        print_summary(num, mines_marked, size);
    }

    return (long) end - (long) start;

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

/*
 * Returns -1 if empty space (clearing required)
 * Returns  0 if safe, no clearing required
 * Returns  1 if bomb...you lose
 */
int uncover(char board) {

    if(board == '*') {
        return 1;
    } else if(board == '0') {
        return -1;
    } else { 
        return 0;
    }

}

void print_summary(int num, int right, int size) {
    printf("\n=================== Mines Summary ===================\n");
    printf("Board Size: %dx%d\n", size, size);
    printf("Number of mines: %d\n", num);
    printf("Difficulty: %d\n", (10 * num) / (size * size));
    printf("Number of mines correctly marked: %d\n", right);
    printf("=====================================================\n");
}

// warning: big honkin' recursive function ahead
void clear_area(int r, int c, char ** view, char ** board, Cell ** head, int size) {

    for(int sr = r - 1; sr <= r + 1; ++sr) {
        for(int sc = c - 1; sc <= c + 1; ++sc) {

            // make sure they're safe to dereference
            if((sr >= 0 && sc >= 0) && (sr < size) && (sc < size)) {

                if(board[sr][sc] == '0') {
                    if(add_cell(head, sr, sc)) {
                        clear_area(sr, sc, view, board, head, size);
                    }
                } else {
                    view[sr][sc] = board[sr][sc];
                }
            }
        }
    }

}

bool add_cell(Cell ** handle, int r, int c) {
    
    Cell * head = *handle; // will be changed
    Cell * front = *handle; // want to keep record of

    while(head) {
        
        if(head->r == r && head->c == c) {
            return false;
        }        

        head = head->next;

    }

    // if we get here, the cell wasn't found
    *handle = malloc(sizeof(Cell));
    (*handle)->next = front;
    (*handle)->r = r;
    (*handle)->c = c;

    return true;

}

void destroy_cells(Cell ** handle) {

    Cell * node = *handle;

    while(node) {
        *handle = node->next;
        free(node);
        node = *handle;
    }

}
