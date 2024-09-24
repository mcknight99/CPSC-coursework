#define ROWS 3
#define COLS 4

#include <stdio.h>
#include <stdlib.h>

int main() {
    int** arr;
    int count = 0,i,j;
    /*This line allocates the memory for the upper layer of pointers (size of an int pointer times the amount of rows)     */
    arr = (int**)malloc(sizeof(int*)*ROWS);
    /* This line of code allocates space for all of the data (size of an int times the rows times the cols)*/
    arr[0] = (int*)malloc(sizeof(int)*ROWS*COLS);
    /* This for loop points the remaining pointers to the appropriate place in memory*/
    for(i = 1; i < ROWS; ++i) {
        arr[i] = arr[i-1] + COLS;
    }
    /* This for loop initializes the data*/
    for(i = 0; i < ROWS; ++i) {
        for(j = 0; j < COLS; ++j) {
            arr[i][j] = count++;
        }
    }
    /* This for loop prints the data*/
    for(i = 0; i < ROWS; ++i) {
        for(j = 0; j < COLS; ++j) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    /* This line of code frees the memory*/
    free(arr[0]);
    free(arr);
    return 0;
}