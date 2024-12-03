/********************************/
/*Samuel Pupke                  *
/*CPSC2310 Fall 2024            *
/*UserName: spupke              *
/*Instructor:Dr. Yvon Feaster   *
/********************************/


#include <stdio.h> // for FILE
#include <stdlib.h> // for malloc
#include <stdbool.h> // for bool

int** readFile(FILE* fp, int *size);
void printMatrix (int** mat, int num);
bool isRightDiagonal(int size, int row, int col);
bool isLeftDiagonal(int row, int col);
int calculateVal(int** mat, int size);