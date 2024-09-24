// This is a problem that involves dynamically allocating memory in 1 dimension but accessing the data as if it were 2 dimensional.
#include <stdlib.h>
#include <stdio.h>

// Assume the following variables are created and initialized.
int main()
{
    int row = 3;
    int col = 4;

    int *data = malloc(row * col * sizeof(int));

    int i, j, count = 0;

    // Fill in the appropriate information in the nested for loops  to initialize the dynamically allocated memory for data:

    for (i = 0; i < row; i++)

    {

        for (j = 0; j < col; j++)

        {

            data[sizeof(int)*i*col + j] = count++;
        }
    }

    // Fill in the appropriate information in the nested for loops to print the data in the dynamically allocated memory for data:
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
        {
            printf("%d ", data[sizeof(int)*i*col + j]);
        }
        printf("\n");
    }

    // Free the dynamically allocated memory
    free(data);
}
// What is the code to give the dynamically allocated memory back to the operating system?
// free(data);