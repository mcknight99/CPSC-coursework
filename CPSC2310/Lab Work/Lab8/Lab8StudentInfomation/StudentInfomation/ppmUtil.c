#include "ppmUtil.h"
/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

/*

Your program should be able to ignore the comments from each of the
ppm files provided with comments. Your program should read the header
information and the pixel values of the image, storing each. Then use
the stored information to create a new ppm image with no comments in the header

You will use fscanf and fprintf when reading and writing the header information
You are required to use fread and fwrite for reading and writing the pixel data

You will implement your functions in ppmUtil.c provided.
You will need several functions for this project (some mentioned above).
Functions should be small and do very specific task.
Rule of thumb I was taught is: if all of the lines of code in your function
cannot be seen on your screen, without scrolling,
your function is doing to much, break it up.
Points will be deducted if any of your functions are doing to many tasks.
*/

// A function that reads the header information from a file.
// Your program should be able to ignore the comments (#) from
// each of the ppm files provided with comments.
void readHeader(FILE *in, header_t *header)
{
    // Read the type of the image
    fscanf(in, "%s ", header->type);
    ignoreComments(in);
    // Read the width, height, and maxVal of the image
    fscanf(in, "%u ", &header->width);
    ignoreComments(in);
    fscanf(in, "%u ", &header->height);
    ignoreComments(in);
    fscanf(in, "%u", &header->maxVal);
    ignoreComments(in);
    seekToPixelData(in);

    // Print the header information
    printf("Type: [%s]\nWidth: [%u]\nHeight: [%u]\nMaxVal: [%u]\n", 
    header->type, header->width, header->height, header->maxVal);
}

// A function that reads pixel values from a file.
// You are required to use fread to read the pixel values from the input file
// (a .ppm file). This only requires one call to fread. You have access to
// all information needed to read all pixel values with one call to fread.
// Points will be deducted if you use multiple calls to fread.
void readPixels(FILE *in, pixel_t *pixel, header_t *header)
{
    ignoreComments(in);
    // Read the pixel values from the input file
    fread(pixel, sizeof(pixel_t), header->width * header->height, in);
}

// A function that writes the header information to a file.
void writeHeader(FILE *out, header_t *header)
{
    // Write the type of the image
    fprintf(out, "%s\n", header->type);
    // Write the width, height, and maxVal of the image
    fprintf(out,"%u %u\n%u\n",header->width, header->height,header->maxVal);
}

// A function that writes pixel values to a file.
// You are required to use fwrite to write the pixel values to the output
// file (a .ppm file). There should be only one call to fwrite.
// Points will be deducted if multiple calls to fwrite are used.
void writePixels(FILE *out, pixel_t *pixel, header_t *header)
{
    // Write the pixel values to the output file
    fwrite(pixel, sizeof(pixel_t), header->width * header->height, out);
}

// A function that dynamically allocates memory for the pixel data.
pixel_t *allocateMemory(header_t *header)
{
    // Dynamically allocate memory for the pixel data
    return (pixel_t *)malloc(sizeof(pixel_t) * header->width * header->height);
}

// A function that returns, to the system, the dynamically allocated memory.
void freeMemory(pixel_t *pixel)
{
    // Free the dynamically allocated memory
    free(pixel);
}

// A function that checks for and ignores comments in the ppm header.
// this should leave the cursor at the beginning of the next line that
// is not a comment, allowing for an easy read of the next value
void ignoreComments(FILE *in)
{
    // Check for comments in the ppm header
    while (fgetc(in) == '#')
    {
        // Ignore the comment until the next line
        while (fgetc(in) != '\n')
            ;
    }
    // Move the cursor to the beginning of the next line that is not a comment
    // Moves back because fgetc moves the cursor forward, causing the next
    // read to be off by one
    fseek(in, -1, SEEK_CUR);

}

// function to put cursor before next line
// similar to ignoreComments in the sense that it seeks backwards
// but this just does it to get in front of the pixel data
void seekToPixelData(FILE *in)
{
    // Check for comments in the ppm header
    while (fgetc(in) == '\n')
    {
    }
    // Move the cursor to the beginning of the next line that is not a comment
    // Moves back because fgetc moves the cursor forward, causing the next
    // read to be off by one
    fseek(in, -1, SEEK_CUR);
}