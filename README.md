# c-mines
A C implementation of a CLI version of Minesweeper. 

```
Usage: ~$ ./c-mines len num
```

| Parameter | Meaning |
| --------- | ------- |
| len       | the integer dimension of one side of the square board |
| num       | the number of mines on the board |

## Compilation with GCC
So I don't forget how to do it 2 months later. I could make a 
makefile but copy-pasta is way easier.
```
~$ gcc -Wall -Wextra -pedantic -ggdb -std=c99 c-mines.c -lncurses -o mines
```
