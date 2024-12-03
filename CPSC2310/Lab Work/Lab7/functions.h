/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

#define MONTH_L 25
#define DAY_L 10
#define YEAR_L 10
#define FIRST_L 50
#define LAST_L 50
#define MAJOR_L 10
#define PREF_L 15
#define MAX_L 100
#define N_ASTERISKS 80

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
functions.h

This file contains the declaration of the structs, all #includes, 
    and the function prototypes. 
You must also include header guards. Before each function prototype, 
you must have a detailed description of what the overall function does. 
What the parameters were and what is being returned. 
You are required to have this type of comment block before each function 
    in the .h file.

Structs:
Each set of data will be stored in a linked list. You must use two structs; 
one for the birthday and one that will be used as a node for the linked list. 
You will define a struct for the linked list that will have the following data
     members:
	Character arrays for the first name(size 50), 
    last name(size 50), major (size 10), and class standing(size 15).
        **The survey didn't have class standing on it, 
        so we're using preferred contact method instead
	An instance of the struct that represents the birthday
	A pointer keeping track of the next node in the list

*/

// Here is an example of an overall function description.
/* Parameters: img - image_t pointer array holding the image data for
 *                     each of the input files
 * Return:        output - image_t struct containing output image data
 * This function averages every pixels rbg values from each of the 
 * input images and puts those averages into a single output image
 */

/*

node_t* createList(FILE*, node_t**) – 
This function is called in main (driver.c) and starts the process of 
creating the list. The first argument will be a file pointer to your 
input file and the second will be a double pointer to the head of your 
list. Use a loop to read from your input file, calling readNodeInfo 
for each node and then calling add to add that node to the list.  
After all of the information from the input file has been added to the 
list, return a pointer to the list.
 
void add(node_t** node, node_t** head) – 
This is the function used to add the node to the linked list. 
You will take in two parameters, a double pointer to the node 
you want to add and a double pointer to the head of the list. 
You should check if the list is empty, if not add the node to the list. 
You are required to print out the data in the order it is read in so add 
the node to the end of your linked list. 

node_t* readNodeInfo(FILE* input) – 
(called by createList) This function will read the data from the 
input file, returning a pointer to the populated node. 
Use malloc to allocate the memory for the node that will 
eventually be added to the linked list. Using scanset conversion, 
read the data and store it in the node allocated. 
(You must use scanset conversion to read ALL the data, not just part 
of the data.) Points will be deducted if you do not use scanset to 
read all input data. HINT: Scanset reads information as a character, 
but you will be reading in some data that are numbers. You will need 
to use the C provided function “atoi” ascii to int to convert the 
character to integers.  

void printList(FILE*, node_t*) – 
This function prints, to the output file, the data from the list. 
If the list is empty you are required to print a message, to stderr, 
indicating the list is empty and exit the program. If the list is not 
empty you are to print LIST INFO: then print the information for each 
node in the list. See example below for the required format. Described
below is a function called printBorder which prints a line of 80 asterisk
“*”. You will call this function before printing the list and after 
printing the list. 
An example of the print format: 
Example: 
********************************************************************************
LIST INFO:
Name: Jane Doe
Date of Birth:  January 1, 2000
Degree:  CIS-BS
Preferred method of communication: email

Then the next info

Then the next info
********************************************************************************

void printBorder(FILE*) – 
This function prints, to the output file, 80 asterisk “*”.

void deleteList(node_t** ) – 
After you are finished with the nodes in the list you need to 
give the memory back to the system. That is what this function does. 
*/

// Struct for the birthday
typedef struct {
    char month[MONTH_L];
    char day[DAY_L];
    char year[YEAR_L];
} birthday_t;

// Struct for the linked list
typedef struct node {
    char firstName[FIRST_L];
    char lastName[LAST_L];
    char major[MAJOR_L];
    char preferredContact[PREF_L];
    birthday_t birthday;
    struct node* next;
} node_t;

/* Parameters: FILE - file pointer to the input file
               node_t - double pointer to the head of the list
 * Return:     node_t - pointer to the head of the list
 * This function starts the process of creating the list.
 */
node_t* createList(FILE* file, node_t** head);

/* Parameters: node_t - double pointer to the node you want to add
               node_t - double pointer to the head of the list
 * Return:     void
 * This function is used to add the node to the linked list.
 */
void add(node_t** node, node_t** head);

/* Parameters: FILE - file pointer to the input file
 * Return:     node_t - pointer to the populated node
 * This function reads the data from the input file, 
 * returning a pointer to the populated node.
 */
node_t* readNodeInfo(FILE* file);

/* Parameters: FILE - file pointer to the output file
               node_t - pointer to the head of the list
 * Return:     void
 * This function prints, to the output file, the data from the list.
 */
void printList(FILE* file, node_t* head);

/* Parameters: FILE - file pointer to the output file
 * Return:     void
 * This function prints, to the output file, N_ASTERISKS asterisks.
 */
void printBorder(FILE* file);

/* Parameters: node_t - double pointer to the head of the list
 * Return:     void
 * This function gives the memory back to the system.
 */
void deleteList(node_t** head);

#endif