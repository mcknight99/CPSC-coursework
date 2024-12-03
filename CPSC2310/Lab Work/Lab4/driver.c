/********************************/
/*Samuel Pupke                  *
/*CPSC2310 Fall 2024            *
/*UserName: spupke              *
/*Instructor:Dr. Yvon Feaster   *
/********************************/


#include <stdio.h> 
#include <stdlib.h>
#include "functions.h" // need to include the header file

int main(int argc, char** argv)
{
    printf("cheking command line arguments\n");

    if (argc < 2)
    { 
        printf("not enought argument: ./exe filename\n"); 
        exit(-1);
    }

    FILE* fp = fopen(argv[1], "r"); //needed to be read
    if(fp==NULL)
    {
        printf("fp did not open\n"); 
        exit(-1);
    }
    int size = 0;
    int **mat = readFile(fp, &size);
    printMatrix(mat, size);
    printf("Total = %d\n", calculateVal(mat, size));

    return 0;
}
