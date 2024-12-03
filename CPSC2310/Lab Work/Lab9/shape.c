/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

/*
This file should implement all functions specific to the shape.  

Circle info: 
A circle has one point (x and y) that represents the center of the circle.
It has an integer value for the radius.  

The radius of the Circle. 
The third line input line represents the RGB color values for the Circle.  
With the information contained in the input file you are going to create 
    a ppm image that has one circle, like the following. 
    You are allowed to define the color of the background. 
    The color, size, and placement of the circle is determined by 
    the input file.  

Given the size of the image you are going to determine the color of 
each pixel in your image. This will be done by checking each pixel 
location in the image to determine if the location is within the 
circle’s parameters. If it is, the pixel color will be the color 
defined in the input file. If it is not within the circle’s parameters, 
the color will be a predefined color of your choice. 

Circle Formula 
A circle is defined by a point that represents the center of the circle 
and an integer that represents the radius of the circle. 
With this information and the point that represents a pixel on a plane, 
determining if the point is within the circle is a relatively simple formula.  

If the square root of ((x – center x)2  + (y – center y)2) 
is less than the radius, then the point is within the circle. 
 
*/

#include "shape.h"

/* Parameters: outFile - file pointer to the output file
 *             header - pointer to the header struct
 *             circle - pointer to the circle struct
 * Return: void
 * This function writes the circle to the output file
 */
void writeCircle(FILE *outFile, Header *header, Circle *circle, Pixel *b) {
    writeHeader(outFile, header);
    for (int y = 0; y < header->height; y++) {
        for (int x = 0; x < header->width; x++) {
            Point point = {x, y};
            Pixel pixel = getPixelColor(point, circle, b);
            writePixel(outFile, &pixel);
        }
    }
}

/* Parameters: x - x coordinate of the pixel
 *             y - y coordinate of the pixel
 *             circle - pointer to the circle struct
 *             background - pointer to the pixel struct
 * Return: Pixel
 * This function returns the color of the pixel by using isWithinCircle
 */
Pixel getPixelColor(Point point, Circle *circle, Pixel *background) {
    Pixel pixel;
    if (isWithinCircle(&point, circle)) {
        pixel = circle->color;
    } else {
        pixel = *background;
    }
    return pixel;
}

/* Parameters: pixel - pointer to the pixel struct
 *             circle - pointer to the circle struct
 * Return: int
 * This function checks if the pixel is within the circle
 */
int isWithinCircle(Point *point, Circle *circle) {
    int px = point->x;
    int py = point->y;
    int center_x = circle->center.x;
    int center_y = circle->center.y;
    int radius = circle->radius;
    return (sqrt(pow(px - center_x, 2) + pow(py - center_y, 2)) < radius);
}

/* Parameters: inFile - file pointer to the input file
 *             header - pointer to the header struct
 *             circle - pointer to the circle struct
 * Return: void
 * This function reads the header and circle information from the input file
 */
void readCircleHeader(FILE *inFile, Header *header, Circle *c) {
    fscanf(inFile, "%s", header->magicNum);
    fscanf(inFile, "%d %d", &header->width, &header->height);
    fscanf(inFile, "%d", &header->maxColor);
    fscanf(inFile, "%d %d %d", &c->center.x, &c->center.y, &c->radius);
    fscanf(inFile, "%hhu %hhu %hhu", &c->color.r, &c->color.g, &c->color.b);
}