#include <stdlib.h>
// Write code to dynamically allocate memory for a 2D array of integers and then free the memory
// (using only one call to malloc and free).

int main() {
    int row = 3;
    int col = 4;

    // Allocate memory for the 2D array
    int* data = malloc(row * col * sizeof(int));

    //for(int i = 0; i < row; i++)
    //{
    //    for(int j = 0; j < col; j++)
    //    {
    //        data[i * col + j] = i * col + j;
    //    }
    //}
    
    // Free the memory for the 2D array
    free(data);
    return 0;
}