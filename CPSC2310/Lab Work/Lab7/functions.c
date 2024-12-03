/********************************
 *Samuel Pupke                  *
 *CPSC 2310 Fall 24             *
 *UserName: spupke              *
 *Instructor:  Dr. Yvon Feaster *
*********************************/

/*
Requirements:

Scanset conversion

Most of you know how to read and write information using scanf/printf or 
fscanf/fprintf.  
For this assignment, you are going to read data from a comma delimited file 
using the concept of scanset conversion. 
In the C review recording I discussed and reviewed examples of 
scanset conversion. 
You are required to use scanset conversion when reading ALL of the data
*/

#include "functions.h"

/* Parameters: FILE - file pointer to the input file
               node_t - double pointer to the head of the list
 * Return:     node_t - pointer to the head of the list
 * This function starts the process of creating the list.
 */
node_t* createList(FILE* file, node_t** head) {
    // This function is called in main (driver.c) and starts the process 
    // of creating the list. The first argument will be a file pointer 
    // to your input file and the second will be a double pointer to 
    // the head of your list. Use a loop to read from your input file, 
    // calling readNodeInfo for each node and then calling add to add 
    // that node to the list.  After all of the information from the 
    // input file has been added to the list, return a pointer to the list.
    // Remember, we must use scanset conversion to read ALL the data. (fscanf)
    while(!feof(file)) {
        node_t* node = readNodeInfo(file);
        add(&node, head);
    }
    return *head;
}

/* Parameters: node_t - double pointer to the node you want to add
               node_t - double pointer to the head of the list
 * Return:     void
 * This function is used to add the node to the linked list.
 */
void add(node_t** node, node_t** head) {
    // This is the function used to add the node to the linked list. 
    // You will take in two parameters, a double pointer to the node 
    // you want to add and a double pointer to the head of the list. 
    // You should check if the list is empty, if not add the node to 
    // the list. You are required to print out the data in the order 
    // it is read in so add the node to the end of your linked list. 
    // If the list is empty, set the head to the node
    // If the list is not empty, add the node to the end of the list
    if (*head == NULL) {
        *head = *node;
    } else {
        node_t* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = *node;
    }
}

/* Parameters: FILE - file pointer to the input file
 * Return:     node_t - pointer to the populated node
 * This function reads the data from the input file, 
 * returning a pointer to the populated node.
 */
node_t* readNodeInfo(FILE* f) {
    // (called by createList) This function will read the data from the 
    // input file, returning a pointer to the populated node. Use malloc 
    // to allocate the memory for the node that will eventually be added 
    // to the linked list. Using scanset conversion, read the data and 
    // store it in the node allocated. (You must use scanset conversion 
    // to read ALL the data, not just part of the data.) Points will be 
    // deducted if you do not use scanset to read all input data. HINT: 
    // Scanset reads information as a character, but you will be reading 
    // in some data that are numbers. You will need to use the C provided 
    // function “atoi” ascii to int to convert the character to integers.  
    node_t* node = (node_t*)malloc(sizeof(node_t));
    // Use scanset conversion to read all the data
    // Not every entry is valid, so we need to validate inputs
    // (some rows have missing data)
    // (1 missing last name-74, 2 missing months-45-108, 1 missing day-82)
    // instructed to remove the entry on line 74
    char buf[MAX_L], z[FIRST_L], y[LAST_L], 
    x[MONTH_L], w[DAY_L], v[YEAR_L], u[MAJOR_L], t[PREF_L];
    //fscanf(f,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",z,y,x,w,v,u,t);
    // Idea: first scan in the whole line until the new line from the file
    fscanf(f, "%[^\n]\n", buf);
    // then scan in the data from the buffer
    sscanf(buf,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",z,y,x,w,v,u,t);
    // the above solution fixes the issue of reading entries missing data

    // clean spaces from the data
    // this makes printing to the file easier and more normalized
    // one downside: removes spaces in double names
    // cannot use the \0 at 1st space b/c it is the null terminator
    // need to first left shift data before replacing the 1st space with \0
    for(int i = 0; i<FIRST_L; i++) {
        if(z[i] == ' ') {
            for(int j = i; j < FIRST_L; j++) {
                z[j] = z[j+1];
            }
        }
    }
    for(int i = 0; i < LAST_L; i++) {
        if(y[i] == ' ') {
            for(int j = i; j < LAST_L; j++) {
                y[j] = y[j+1];
            }
        }
    }
    for(int i = 0; i < MONTH_L; i++) {
        if(x[i] == ' ') {
            for(int j = i; j < MONTH_L; j++) {
                x[j] = x[j+1];
            }
        }
    }
    for(int i = 0; i < DAY_L; i++) {
        if(w[i] == ' ') {
            for(int j = i; j < DAY_L; j++) {
                w[j] = w[j+1];
            }
        }
    }
    for(int i = 0; i < YEAR_L; i++) {
        if(v[i] == ' ') {
            for(int j = i; j < YEAR_L; j++) {
                v[j] = v[j+1];
            }
        }
    }
    for(int i = 0; i < MAJOR_L; i++) {
        if(u[i] == ' ') {
            for(int j = i; j < MAJOR_L; j++) {
                u[j] = u[j+1];
            }
        }
    }
    for(int i = 0; i < PREF_L; i++) {
        if(t[i] == ' ') {
            for(int j = i; j < PREF_L; j++) {
                t[j] = t[j+1];
            }
        }
    }
    // once each string has been left shifted, we need to delimit the strings
    for(int i = 0; i < FIRST_L; i++) {
        if(z[i] == ' ') {
            z[i] = '\0';
        }
    }
    for(int i = 0; i < LAST_L; i++) {
        if(y[i] == ' ') {
            y[i] = '\0';
        }
    }
    for(int i = 0; i < MONTH_L; i++) {
        if(x[i] == ' ') {
            x[i] = '\0';
        }
    }
    for(int i = 0; i < DAY_L; i++) {
        if(w[i] == ' ') {
            w[i] = '\0';
        }
    }
    for(int i = 0; i < YEAR_L; i++) {
        if(v[i] == ' ') {
            v[i] = '\0';
        }
    }
    for(int i = 0; i < MAJOR_L; i++) {
        if(u[i] == ' ') {
            u[i] = '\0';
        }
    }
    for(int i = 0; i < PREF_L; i++) {
        if(t[i] == ' ') {
            t[i] = '\0';
        }
    }
    // copy the data into the node
    strcpy(node->firstName, z);
    strcpy(node->lastName, y);
    strcpy(node->birthday.month, x);
    strcpy(node->birthday.day, w);
    strcpy(node->birthday.year, v);
    strcpy(node->major, u);
    strcpy(node->preferredContact, t);
    node->next = NULL;
    return node;
}

/* Parameters: FILE - file pointer to the output file
               node_t - pointer to the head of the list
 * Return:     void
 * This function prints the data from the list to the output file.
 */
void printList(FILE* file, node_t* head) {
    // This function prints, to the output file, the data from the list. 
    // If the list is empty you are required to print a message, to stderr, 
    // indicating the list is empty and exit the program. If the list is not 
    // empty you are to print LIST INFO: then print the information for each 
    // node in the list. See example below for the required format. Described 
    // below is a function called printBorder which prints a line 
    // of 80 asterisk “*”. 
    // You will call this function before printing the list and
    // after printing the list. 
    // An example of the print format: 
/*
********************************************************************************
LIST INFO:
Name: Jane Doe
Date of Birth:  January 1, 2000
Degree:  CIS-BS
Preferred method of communication: email

Then the next info

Then the next info
********************************************************************************
*/
    // If the list is empty, print a message to stderr and exit the program
    // If the list is not empty, print LIST INFO: then print the information
    // for each node in the list
    // Call printBorder before printing the list
    // Call printBorder after printing the list
    if (head == NULL) {
        fprintf(stderr, "The list is empty.\n");
        exit(1);
    }
    printBorder(file);
    fprintf(file, "LIST INFO:\n");
    for(node_t* current = head; current != NULL; current = current->next) {
        // skip the empty nodes
        if(current->firstName[0] == '\0' || current->lastName[0] == '\0') {
            continue;
        }
        // i would also skip the bad data nodes but that wasn't required
        // fun data facts:
        // 3 birthdays on Sep 17, 3 on Jan 21, 4 on March 6
        fprintf(file, "Name: %s %s\n", current->firstName, current->lastName);
        char m[25], d[10], y[10];
        strcpy(m, current->birthday.month);
        strcpy(d, current->birthday.day);
        strcpy(y, current->birthday.year);
        fprintf(file, "Date of Birth: %s %s, %s\n", m, d, y);
        fprintf(file, "Degree: %s\n", current->major);
        char c[15];
        strcpy(c, current->preferredContact);
        fprintf(file, "Preferred method of communication: %s\n\n", c);
    }
    printBorder(file);
}

/* Parameters: FILE - file pointer to the output file
 * Return:     void
 * This function prints, to the output file, N_ASTERISKS asterisks.
 */
void printBorder(FILE* file) {
    for(int i = 0; i < N_ASTERISKS; i++) {
        fprintf(file, "*");
    }
    fprintf(file, "\n");
}

/* Parameters: node_t - double pointer to the head of the list
 * Return:     void
 * This function frees the memory from the linked list.
 */
void deleteList(node_t** head) {
    // After you are finished with the nodes in the list you need to give 
    // the memory back to the system. That is what this function does.
    // Use a loop to free the memory for each node in the list
    // Set the head to NULL
    for(node_t* current = *head; current != NULL; current = current->next) {
        // Free the memory for each node in the list
        free(current);
    }
}
