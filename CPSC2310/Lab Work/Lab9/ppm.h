/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

/*
The input file will have the following information in this order:  
P6 800 800 255 
400 400 100 
255 0 0 
Explanation of the input file:  
The first line is the header of the ppm image you are going to create. 
The second line of data represents: 
The point values for x and y that represents the center of the circle. 
Remember x will represents width and y represents height. See graph below.

This file should have function prototypes related to ppm images. 
It should also define any structs related to ppms.  
1. Struct to represent the pixels.  
2. Struct to represent the header.  
Functions: 
1. A function to write the header to the the output file. 
2. A function to write one pixel to the output file.  
*/

#ifndef PPM_H
#define PPM_H

#include <stdio.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

typedef struct {
    char magicNum[3];
    int width;
    int height;
    int maxColor;
} Header;

#include "shape.h"

/* Parameters: outFile - file pointer to the output file
 *             header - pointer to the header struct
 * Return: void
 * This function writes the header to the output file
 */
void writeHeader(FILE *outFile, Header *header);

/* Parameters: outFile - file pointer to the output file
 *             pixel - pointer to the pixel struct
 * Return: void
 * This function writes one pixel to the output file
 */
void writePixel(FILE *outFile, Pixel *pixel);



#endif // PPM_H