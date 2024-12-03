#ifndef PPMUTIL_H
#define PPMUTIL_H

/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Pixel
{
    unsigned char r, g, b;
}pixel_t;

typedef struct Header
{
    char type[3];
    unsigned int width;
    unsigned int height;
    unsigned int maxVal;
}header_t;

/*

I will provide a file named ppmUtil.h that will contain two structs, 
one to represent the pixels of a ppm image and one to represent the 
data that makes up the header of a ppm image. You will need to create 
several functions.

At a minimal you will need functions that do the following:

A function that reads the header information from a file.
A function that reads pixel values from a file. 
    You are required to use fread to read the pixel values from the 
    input file (a .ppm file). This only requires one call to fread. 
    You have access to all information needed to read all pixel values 
    with one call to fread. Points will be deducted if you use multiple 
    calls to fread. 
A function that writes the header information to a file. 
A function that writes pixel values to a file. 
    You are required to use fwrite to write the pixel values to the 
    output file (a .ppm file). There should be only one call to fwrite. 
    Points will be deducted if multiple calls to fwrite are used. 
A function that dynamically allocates memory for the pixel data.
A function that returns, to the system, the dynamically allocated memory. 
A function that checks for and ignores comments in the ppm header. 
*/

//Your function prototypes will placed here.

// A function that reads the header information from a file.
void readHeader(FILE* in, header_t* header);
// A function that reads pixel values from a file. You are required to use fread to read the pixel values from the input file (a .ppm file). This only requires one call to fread. You have access to all information needed to read all pixel values with one call to fread. Points will be deducted if you use multiple calls to fread. 
void readPixels(FILE* in, pixel_t* pixel, header_t* header);
// A function that writes the header information to a file. 
void writeHeader(FILE* out, header_t* header);
// A function that writes pixel values to a file. You are required to use fwrite to write the pixel values to the output file (a .ppm file). There should be only one call to fwrite. Points will be deducted if multiple calls to fwrite are used. 
void writePixels(FILE* out, pixel_t* pixel, header_t* header);
pixel_t* allocateMemory(header_t* header);
// A function that returns, to the system, the dynamically allocated memory. 
void freeMemory(pixel_t* pixel);
// A function that checks for and ignores comments in the ppm header. 
void ignoreComments(FILE* in);
// A function that seeks to the pixel data in the ppm file.
void seekToPixelData(FILE* in);
#endif
