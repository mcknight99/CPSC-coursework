/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

/*
You are required to use header guards in each of the .h files. 
Points will be deducted if header guards are not used. 

This file will have function protypes related to shapes. 
It should also contain at least 2 structs 
1. Struct to represent a point: x and y values. 
These should be integers not floats.  
2. Struct to represent a circle. This should contain the following:  
    a. a point that represents the center of a circle 
    b. an integer that represents the radius for the circle 
    c. a pixel that represents the color values for the circle 

When designing your functions, they must be small and do specific task. 
Large clunky functions will result in a reduction of points.  
Functions: 

A function that will read from the input file that contains the 
    information for the header and the data that describes the circle: 
    the center point, radius, circle color. 

A function that will loop through each row and column 
    (each point (pixel) location) of the image checking if the point (pixel)
    in the image is within the defined circle. If the pixel is within the 
    circle, the pixel color will be set to the circles color and written 
    to the output ppm image. If it is not within the circle, the color will 
    be the default color defined by you and written to the output ppm image.  
Hint:  I suggest number 2 above be broken into 2 functions.  
One to loop through the pixels and for each pixel call another function 
to check if the pixel is within the circle returning true or false. 
If true the color is the color of the circle, 
if false the pixel is the color of the background which you will define.  
*/

#ifndef SHAPE_H
#define SHAPE_H

#include <math.h>
#include "ppm.h"

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point center;
    int radius;
    Pixel color;
} Circle;

/* Parameters: inFile - file pointer to the input file
 *             header - pointer to the header struct
 *             circle - pointer to the circle struct
 * Return: void
 * This function reads the header and circle information from the input file
 */
void readCircleHeader(FILE *inFile, Header *header, Circle *circle);

/* Parameters: outFile - file pointer to the output file
 *             header - pointer to the header struct
 *             circle - pointer to the circle struct
 * Return: void
 * This function writes the circle to the output file
 */
void writeCircle(FILE*, Header*, Circle*, Pixel*);

/* Parameters: x - x coordinate of the pixel
 *             y - y coordinate of the pixel
 *             circle - pointer to the circle struct
 *             b - pointer to the pixel struct for the background color
 * Return: Pixel
 * This function returns the color of the pixel using isWithinCircle
 */
Pixel getPixelColor(Point point, Circle *circle, Pixel *background);

/* Parameters: point - pointer to the point struct
 *             circle - pointer to the circle struct
 * Return: int
 * This function returns 1 if the point is within the circle, 0 otherwise
 */
int isWithinCircle(Point *point, Circle *circle);

#endif // SHAPE_H