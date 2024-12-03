/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

/*
This file is where main will live. 
This file should have minimal amount of code.  
1. Open the files 
2. Use assert to check if the files opened successfully 
3. Call a function to read the information from the input file 
4. Call a function to write the header to the output file 
5. Call a function that will create the circle 
6. Close the files 
*/

#include "ppm.h"
#include "shape.h"
#include <assert.h>

#define BACKGROUND_R 255
#define BACKGROUND_G 255
#define BACKGROUND_B 255


int main(int argc, char *argv[]) {
    assert(argc == 3);
    FILE *inFile = fopen(argv[1], "r");
    FILE *outFile = fopen(argv[2], "w");
    assert(inFile != NULL);
    assert(outFile != NULL);

    Pixel backgroundColor = {BACKGROUND_R, BACKGROUND_G, BACKGROUND_B};

    Header header;
    Circle circle;
    readCircleHeader(inFile, &header, &circle);
    writeCircle(outFile, &header, &circle, &backgroundColor);

    fclose(inFile);
    fclose(outFile);
    return 0;
}