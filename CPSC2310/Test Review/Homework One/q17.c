//#include <stdio.h>
#include <stdlib.h>

 // Write code to dynamically allocate memory for a 2D array of integers 
 // (multiple calls to malloc) and then free the memory (multiple calls to free).

int main() {
    int row = 3;
    int col = 4;
    
    // Allocate memory for the 2D array
    int** data = malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++) {
        data[i] = malloc(col * sizeof(int));
    }

    // Free the memory for the 2D array
    for (int i = 0; i < row; i++) {
        free(data[i]);
    }
    free(data);

    return 0;
}