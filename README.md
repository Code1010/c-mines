# c-mines
A C implementation of Minesweeper using ncurses. 

```
Usage: ~$ ./mines len num
```

| Parameter | Meaning |
| --------- | ------- |
| len       | the integer dimension of one side of the square board |
| num       | the number of mines on the board |

## Compilation with GCC
I've included a simple makefile. This is basically all it does. 
```
~$ gcc c-mines.c -lncurses -o mines
```
