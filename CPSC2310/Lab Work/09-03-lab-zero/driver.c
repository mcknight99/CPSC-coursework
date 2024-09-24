//#include "function.h"
#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdio.h>
void printName();
#endif

void printName() {
    printf("Hello World! My name is Samuel Pupke.\n");
}

int main() {
    printName();
    return 0;
}