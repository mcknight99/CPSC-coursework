#include <stdlib.h>
// Write code to dynamically allocate memory for a 2D array of integers and then free the memory
// (using only two calls to malloc and free each).

int main() {
    int row = 3;
    int col = 4;

    // Allocate memory for the 2D array
    int** data = malloc(row * sizeof(int*));
    data[0] = malloc(row * col * sizeof(int));

    //for (int i = 1; i < row; i++) {
    //    data[i] = data[0] + i * col;
    //}
    
    // Free the memory for the 2D array
    free(data[0]);
    free(data);
    return 0;

}