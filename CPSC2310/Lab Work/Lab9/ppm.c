/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

/*
This file has the implementation of the functions in ppm.h. 
*/

#include "ppm.h"

/* Parameters: outFile - file pointer to the output file
 *             header - pointer to the header struct
 * Return: void
 * This function writes the header to the output file
 */
void writeHeader(FILE *outFile, Header *header) {
    fprintf(outFile, "%s\n", header->magicNum);
    fprintf(outFile, "%d %d\n", header->width, header->height);
    fprintf(outFile, "%d\n", header->maxColor);
}


/* Parameters: outFile - file pointer to the output file
 *             pixel - pointer to the pixel struct
 * Return: void
 * This function writes one pixel to the output file
 */
void writePixel(FILE *outFile, Pixel *pixel) {
    fprintf(outFile, "%c%c%c", pixel->r, pixel->g, pixel->b);
}

