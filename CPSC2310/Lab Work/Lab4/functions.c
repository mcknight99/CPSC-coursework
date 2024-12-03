/********************************/
/*Samuel Pupke                  *
/*CPSC2310 Fall 2024            *
/*UserName: spupke              *
/*Instructor:Dr. Yvon Feaster   *
/********************************/


#include "functions.h" // need to include the header file 

int** readFile(FILE* fp, int *size)
{
    fscanf(fp, "%d", size); //size is already a pointer
    int num = *size;
    int index = 0;
    
    int** mat = (int**)malloc(num * sizeof(int*)); // allocate the right amount of memory
    for(index = 0; index < num; index++) {
        mat[index] = (int*)malloc(num * sizeof(int)); 
    }

    int row = 0; 
    int col = 0;
    for(row = 0; row < num; row++)
    {
        for(col = 0; col < num; col++) // gotta reset the dang thing to zero
        {
            fscanf(fp, "%d", &mat[row][col]); //need to pass the matrix by reference
        }
    }
    return mat;
}


void printMatrix (int** mat, int num)
{
    int row = 0; 
    int col = 0;
    for(row = 0; row < num; row++)
    {
        for(col = 0; col < num; col++)
        {
            printf("%.2d\t", mat[row][col]);
        }
        printf("\n");
    }
    
}

bool isRightDiagonal(int size, int row, int col) {
    return (row + col + 1 == size);
}

bool isLeftDiagonal(int row, int col) {
    return (row == col);
}

int calculateVal(int** mat, int size) {
    int sum = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (!isLeftDiagonal(row, col) && !isRightDiagonal(size, row, col)) {
                sum += mat[row][col];
            }
        }
    }
    return sum;
}