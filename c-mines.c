// c-mines.c
// @author Cody Burrows <dcodyburrows@gmail.com>
// 
// This is an minesweeper-like game made using c and ncurses.
// usage: ./mines len num
// where len and num are integers, and len specifies the side
// length of the lenxlen grid. num specifies the number of
// mines to plant on the grid.
//
// Use wasd to move the cursor, e to mark a mine, and r to 
// uncover a space -- use with caution!
//
// Board Legend:
// #   = covered
// *   = mine
// 0   = empty
// 1-8 = number of adjacent mines
//
// //////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <stdbool.h>
#include "c-mines.h"

/**
  * This function parses cmd line args, and initializes the game
  *
  * @param argc     The number of arguments passed
  * @param argv     The arguments...see above for usage
  *
  * @returns EXIT_SUCCESS most of the time, unless you do something
  *          you shouldn't have.
  */
int main(int argc, char ** argv) {

    // correct number of args
    if(argc == 3) {
        
        int size, num;

        // convert args to base-10 numbers
        size = strtol(argv[1], NULL, 10);
        num = strtol(argv[2], NULL, 10);

        if(size <= 0 || num <= 0) {
            // you don messed this one up bruh
            print_usage();
            return EXIT_FAILURE;
        } else if(size >= 1024) {
            // sorry, too big
            size = 1024;
        } else if(num >= (size * size)) {
            // sorry, too many
            num = (size * size);
        }

        // holds the values for each cell
        // value    /   meaning
        // 0        /   empty
        // 1        /   1 mine touching this square
        // 2        /   2 mines touching this square
        // ...      /   ...
        // 8        /   8 mines touching this square
        // *        /   bomb here 
        char ** board = create_board(size, '0');
        init_board(board, size, num);

        // stores the grid that the user sees
        char ** view = create_board(size, '#');

        // woo hoo
        play_game(board, view, size, num);

        // be a good process...
        destroy_board(view, size);
        destroy_board(board, size);
        return EXIT_SUCCESS;
        
    } else {
        // not enough args bruh
        print_usage();
        return EXIT_FAILURE;
    }
}

/// How do I use this dumb thing again?
void print_usage(void) {

    fprintf(stderr, "Usage: mines len num\n");
    fprintf(stderr, "  where len is the integer length of one side of the board\n");
    fprintf(stderr, "  and   num is the number of mines on the board\n");
    
}

/// Allocates a square array of specified size, filled with a char
char ** create_board(int size, char fill) {

    char ** board = malloc(size * sizeof(char *));

    for(int i = 0; i < size; ++i) {

        board[i] = malloc(size * sizeof(char));
        for(int j = 0; j < size; ++j) {

            // handy
            board[i][j] = fill;

        }

    }

    return board;

}


/// places a number of mines on the board, then calculates adjacent cells
void init_board(char ** board, int size, int num) {

    int count = 0;
    int r, c;

    // for super random mode plus use '48' as the seed
    srand(time(0));
    while(count < num) {
        r = rand() % size;
        c = rand() % size;

        if(board[r][c] != '*') {
            count++;
            board[r][c] = '*';
        }
    }

    // don't do the work but take credit for it
    calculate_cells(board, size);

}

/// Be a good process...
void destroy_board(char ** board, int size) {

    for(int i = 0; i < size; ++i) {

        // read as free bird
        free(board[i]);

    }

    // see comment above
    free(board);

}

/// Calculates the number of mines adjacent to each cell
void calculate_cells(char ** board, int size) {

    // whoa that's a lot of loops
    
    // go through each cell in the board
    for(int r = 0; r < size; ++r) {
        for(int c = 0; c < size; ++c) {

            // if it's not a mine, check for mines around it
            if(board[r][c] != '*') {
                int total = 0;

                // take a look at adjacent cells
                for(int sr = r - 1; sr <= r + 1; ++sr) {
                    for(int sc = c - 1; sc <= c + 1; ++sc) {

                        // make sure they're safe to dereference
                        if((sr >= 0 && sc >= 0) && (sr < size) && (sc < size)) {

                            // bingo!
                            if(board[sr][sc] == '*')
                                total++;
                        }
                    }
                }

                // put the number in as a char
                board[r][c] = total + '0';

            }

        }
    }
}

/// It's simple. Just do everything.
int play_game(char ** board, char ** view, int size, int num) {
    
    time_t start = time(NULL);

    // ncurses init stuff
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
    
    // while you're playing basically
    while(run) {
        
        show_board(view, size, r, c);

        // get your keystrokes from ncurses
        ch = getch();
        if(ch == 'q') {
            // i give up
            run = false;
            dead = true;
        } else if(ch == 'w') {
            // move up
            r--;
        } else if(ch == 's') {
            // move down
            r++;
        } else if(ch == 'a') {
            // move left
            c--;
        } else if(ch == 'd') {
            // move right
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
                    // phew. still alive.
                    view[r][c] = board[r][c];
                    break;

                case 1:
                    view[r][c] = '*';
                    // oh, that's a bomb
                    // *explodes*
                    run = false;
                    dead = true;
                    break;

                case -1:
                    // thank God, it's just dirt
                    view[r][c] = '.';
                    Cell * head = NULL;
                    Cell ** handle = &head;

                    // auto-clear the rest of the dirt 
                    clear_area(r, c, view, board, handle, size);
                    destroy_cells(handle);
                    break;
            }
            
        }

        // wrap around the top/bottom
        if(r < 0)
            r = size - 1;

        // wrap around the edges
        if(c < 0)
            c = size - 1;

        // more wrapping
        r = r % size;
        c = c % size;

        // this is pretty lazy, but it works
        if(mines_marked == num) {
            // win!
            run = false;
            dead = false;
        } 

    }

    // end, denit ncurses
    time_t end = time(NULL);
    refresh();
    curs_set(1);
    endwin();

    // game over...did you win?
    if(dead) {
        // guess not
        printf("\n\nYou died in a glorious ball of fire after only"
                " %ld seconds.\n\n", (long) end - (long) start);
        print_summary(num, mines_marked, size);
    } else {
        // good job
        printf("\n\nNice work! You found all of the mines in %ld seconds. \n\n",
                (long) end - (long) start);
        print_summary(num, mines_marked, size);
    }

    // return total time
    return (long) end - (long) start;

}

/// draws the board, and prints the cursor. duh
void show_board(char ** view, int size, int cursr, int cursc) {
    
    move(0, 0);

    printw("+ - - - - - - - - - - MINES - - - - - - - - - - +\n");
    printw("wasd [move]   e [mark]   r [uncover]   q [quit]\n");
    printw("+ - - - - - - - - - - - - - - - - - - - - - - - +\n\n");

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

/// decides your fate
int uncover(char board) {

    /*
     * Returns -1 if empty space (clearing required)
     * Returns  0 if safe, no clearing required
     * Returns  1 if bomb...you lose
     */

    if(board == '*') {
        return 1;
    } else if(board == '0') {
        return -1;
    } else { 
        return 0;
    }

}

/// a nicely formatted summary
void print_summary(int num, int right, int size) {
    printf("\n=================== Mines Summary ===================\n");
    printf("Board Size: %dx%d\n", size, size);
    printf("Number of mines: %d\n", num);
    printf("Difficulty: %d\n", (10 * num) / (size * size));
    printf("Number of mines correctly marked: %d\n", right);
    printf("=====================================================\n");
}

// warning: big honkin' recursive function ahead

/// clear empty spaces
void clear_area(int r, int c, char ** view, char ** board, Cell ** head, int size) {

    // check the neighbors for empty spaces
    for(int sr = r - 1; sr <= r + 1; ++sr) {
        for(int sc = c - 1; sc <= c + 1; ++sc) {

            // make sure they're safe to dereference
            if((sr >= 0 && sc >= 0) && (sr < size) && (sc < size)) {

                // found an empty cell, recurse on it
                if(board[sr][sc] == '0') {
                    if(add_cell(head, sr, sc)) {
                        clear_area(sr, sc, view, board, head, size);
                        view[sr][sc] = '.';
                    }
                } else {
                    // found a number. print it
                    view[sr][sc] = board[sr][sc];
                }
            }
        }
    }

}

/// insert a cell into the unique list
bool add_cell(Cell ** handle, int r, int c) {
    
    Cell * head = *handle; // will be changed
    Cell * front = *handle; // want to keep record of

    // check for uniqueness
    while(head) {
        
        if(head->r == r && head->c == c) {
            // already there
            return false;
        }        

        head = head->next;

    }

    // if we get here, the cell wasn't found
    *handle = malloc(sizeof(Cell));
    (*handle)->next = front;
    (*handle)->r = r;
    (*handle)->c = c;

    // wasn't there before, but it is now
    return true;

}

/// teach that sucker a lesson, and be a good process...
void destroy_cells(Cell ** handle) {

    Cell * node = *handle;

    while(node) {
        *handle = node->next;
        free(node);
        node = *handle;
    }

}
