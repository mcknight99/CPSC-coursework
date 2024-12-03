/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

/*
driver.c
Main will be included in driver.c.  
Driver.c should have minimal amount of code in it. 
Things you may have in the driver:  
	Create the input and output file pointers. 
    These file names will be given on the command line. 
    Use assert to check for the correct number of command line arguments.
	Open your input and output files and check that they opened correctly. 
    (use assert)
	Call the function createList to create and fill your linked list with 
    input data.
	Call printList to print out the linked list
	Free memory from the linked list by calling deleteList.
Your driver should not have more code than necessary. 
DO NOT include functions.c in your driver.c file. 
As a rule, you should not include “.c” files. 
Points will be deducted if you have excessive code in driver.c
*/

#include "functions.h"
#include <assert.h> // for assert 
#define N_ARGS 3
#define INPUT_ARG 1
#define OUTPUT_ARG 2

int main(int argc, char* argv[]) {
    // Create the input and output file pointers
    FILE* input;
    FILE* output;
    // Use assert to check for the correct number of command line arguments
    // 3 CLAs: driver (0), input (1), output (2)
    assert(argc == N_ARGS);
    // Open your input and output files and check that they opened correctly
    input = fopen(argv[INPUT_ARG], "r");
    assert(input != NULL);
    output = fopen(argv[OUTPUT_ARG], "w");
    assert(output != NULL);
    // Call the function createList to create and fill your linked list 
    //  with input data
    node_t* head = NULL;
    createList(input, &head);
    // Call printList to print out the linked list
    printList(output, head);
    // Free memory from the linked list by calling deleteList
    deleteList(&head);
    // Close the input and output files
    fclose(input);
    fclose(output);
    return 0;
}