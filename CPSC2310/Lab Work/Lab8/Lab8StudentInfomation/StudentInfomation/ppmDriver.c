#include "ppmUtil.h"
/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

/*
The driver should have minimal amount of code in it. 
It should create and open the needed file pointers.
It should create a header_t and a pointer of type pixel_t.
The driver should call a function to read the data from an image.
The driver should also call a function to write the data to an image. 
The driver should also call a function to give the dynamically allocated memory back to the OS and close the file pointers.  
*/


int main(int argc, char** argv)
{
    // Check for the correct number of arguments
    if (argc != 3)
    {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }
    // Create and open the needed file pointers
    FILE* in = fopen(argv[1], "rb");
    FILE* out = fopen(argv[2], "wb");
    // Check for errors in opening the file pointers
    if (in == NULL || out == NULL)
    {
        printf("Error: Unable to open file pointers\n");
        return 1;
    }
    // Create a header_t and a pointer of type pixel_t
    header_t header;
    pixel_t* pixel;
    // Read the data from an image
    readHeader(in, &header);
    pixel = allocateMemory(&header);
    readPixels(in, pixel, &header);
    // Write the data to an image
    writeHeader(out, &header);
    writePixels(out, pixel, &header);
    // Give the dynamically allocated memory back to the OS and close the file pointers
    freeMemory(pixel);
    fclose(in);
    fclose(out);    
    
    return 0;

}

