/*
CPSC 2130 Day One Aug 23 2024
Structures allow us to group various data types under one name, creaint a data type.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct structName {
    int a;
    char b;
    float c;
};

//create by doing this
struct structName ex0;

/*
Suppose I don't want a pesky keyword struct every time i create an instance of the struct. we can use typedef
*/

typedef struct Person {
    char name[50];
    int age;
} person_t;

//can create the instance in two ways
struct Person ex1;
person_t ex2;

/*
Another option to leave off Person at the front
*/
typedef struct {
    char name[50];
    int age;
} person_t;

// no need to use the keyword struct either
person_t ex3;


int main() {
    //initialize the struct
    ex0.a = 10;
    ex0.b = 'a';
    ex0.c = 3.14;

    //initialize the struct
    ex1.age = 20;
    strcpy(ex1.name, "John");

    //initialize the struct
    ex2.age = 30;
    strcpy(ex2.name, "Jane");

    //initialize the struct
    ex3.age = 40;
    strcpy(ex3.name, "Jim");

    printf("ex0.a = %d, ex0.b = %c, ex0.c = %f\n", ex0.a, ex0.b, ex0.c);
    printf("ex1.age = %d, ex1.name = %s\n", ex1.age, ex1.name);
    printf("ex2.age = %d, ex2.name = %s\n", ex2.age, ex2.name);
    printf("ex3.age = %d, ex3.name = %s\n", ex3.age, ex3.name);

    return 0;
}