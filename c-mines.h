// c-mines.h
// @author Cody Burrows
//
// This file contains function and struct definitions
// for c-mines.c
// //////////////////////////////////////////////////

// include guard
#ifndef CMINES_H
#define CMINES_H

/**
  * This struct is used to store data for a cell.
  * It is used for clearing large empty areas.
  */ 
typedef struct cells {
    int r, c;
    
    // it's a linked list because I'm pretty lazy
    struct cells * next;
} Cell;

/**
  * This function is used for clearing area.
  * Its purpose is to insert a cell at the head of a list of 
  * unique cells. A cells are equal if their row and column 
  * are the same.
  *
  * @param  handle      A pointer to the head of the list
  * @param  r           The row
  * @param  c           The column
  *
  * @returns true if the row/column pair passed in is unique,
  *               and was successfully added.
  *          false if the row/column pair was already in the list.
  */ 
bool add_cell(Cell ** handle, int r, int c);

/**
  * This function cleans up the linked cell list. 
  *
  * @param  handle      A pointer to the head of the list
  * @returns nothing
  */ 
void destroy_cells(Cell ** handle);

/**
  * This handy little function prints out the correct usage for
  * command line arguments.
  *
  * @param  void
  *
  * @returns nothing
  */
void print_usage(void);

/**
  * This function places mines on the board. It will
  * ensure that exactly num mines are placed, if there is room.
  *
  * @param board    The board to place mines on
  * @param size     The dimension of the square board
  * @param num      The number of mines to place
  */
void init_board(char ** board, int size, int num);

/**
  * This function initializes an empty board (2d char array) with
  * a specified character.
  *
  * @param size     The dimension of the square board. E.G. 3 would
  *                 create a 3x3 board consisting of 9 spaces.
  * @param fill     The character to initialize each element with.
  *
  * @returns A pointer to a beautiful pre-fab 2d char array
  */
char ** create_board(int size, char fill);

/**
  * Obliterate that sucker.
  *
  * @param  board       The 2d char array to destroy
  * @param  size        The dimension of the square board
  */ 
void destroy_board(char ** board, int size);

/**
  * Calculates the number of mines touching each cell in the board,
  * and updates the values.
  *
  * The board passed in should be the game board that contains values,
  * not the one the player sees.
  *
  * @param  board       The board to calculate and update
  * @param  size        The dimension of the square board
  * 
  * @returns nothing    The board is modified in place
  */
void calculate_cells(char ** board, int size);

/**
  * The juicy part that handles playing the game.
  * 
  * This involves:
  *     - Keeping track of the time
  *     - Init/denit of ncurses mode
  *     - Handling user input (from ncurses)
  *     - Handling marking mines, uncovering spaces, clearing area,
  *       and uncovering a bomb.
  *
  * @param board    The 2d char array that stores mine locations, and the
  *                 values of surrounding cells
  * @param view     The 2d char array that holds the board the user sees
  * @param size     The dimension of board and view
  * @param num      The number of mines on the board
  * 
  * @return the total number of seconds the game was played.
  */
int play_game(char ** board, char ** view, int size, int num);

/**
  * Uses ncurses to display an information banner, and the grid that
  * the player sees. This function also draws the cursor around the
  * active cell.
  *
  * @param view     The board to display on the screen
  * @param size     The dimension of the square board
  * @param cursr    The row that the cursor is on
  * @param cursc    The col that the cursor is on
  *
  * @returns nothing    The board is not modified in any way
  */ 
void show_board(char ** view, int size, int cursr, int cursc);

/**
  * This function determines whether or not you die. Don't make it angry.
  *
  * @param board    The char to test (where the user hit r)
  * @returns 1 if the character is a bomb ('*')
  *         -1 if the character is a space ('0')
  *          0 if the character is anything else
  */
int uncover(char board);

/**
  * Shows some stats, some of which will be useful to a coroner, such as
  * Time of Death, Cause of Death, etc.
  *
  * ...and also board size, number of mines, difficulty, 
  * and the number of bombs you marked right.
  *
  * @param num      The number of bombs on the board
  * @param right    The number of mines you correctly guessed the location of
  * @param size     The dimension of the square board
  *
  * @return nothing it just prints things
  */
void print_summary(int num, int right, int size);

/**
  * This beast clears open areas of the field for you, so you can
  * get on with exploding faster!
  *
  * If you uncover an empty space, it will automatically uncover empty
  * spaces nearby, up to any numbered cells.
  *
  * @param r        The row the user clicked on
  * @param c        The col the user clicked on
  * @param view     The board the user sees
  * @param head     A pointer to a unique list of cells
  * @param size     The dimension of the square board
  */
void clear_area(int r, int c, char ** view, char ** board, Cell ** head, int size);

#endif // end of include guard
