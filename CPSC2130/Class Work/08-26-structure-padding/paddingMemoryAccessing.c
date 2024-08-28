// this will be used to test the memory accessing of the structure by using the pointer to the structure

#include <stdio.h>
#include <stddef.h>

// Struct: struct Example1
//[1___][4][1_2] = 12 bytes
struct Example1
{
    char a;
    int c;
    char b;
    short int d;
};

// Struct: struct Example2
//[1___][4][21_] = 12 bytes
struct Example2
{
    char a;
    int b;
    short int c;
    char d;
};

// Struct: struct Example3
//[4][211] = 8 bytes
struct Example3
{
    int a;
    short int b;
    char c;
    char d;
};

void printStructContents(const void *structPtr, size_t structSize)
{
    const unsigned char *bytes = (const unsigned char *)structPtr;

    printf("[");
    for (size_t i = 0; i < structSize; i++)
    {
        if (bytes[i] == 0)
        {
            printf("_|");
        }
        else
        {
            printf("%c |", bytes[i]);
        }
    }
    printf("] ");
}

int main()
{
    struct Example1 example1 = {'a', 1, 'b', 2};
    struct Example2 example2 = {'c', 3, 4, 'd'};
    struct Example3 example3 = {5, 6, 'e', 'f'};

    // now to go straight to memory and find what's at each byte of data in each struct

    unsigned char *ptr1 = (unsigned char *)&example1;
    unsigned char *ptr2 = (unsigned char *)&example2;
    unsigned char *ptr3 = (unsigned char *)&example3;

    printf("Size of Example1: %zu", sizeof(example1));
    printf("\nExample1: ");
    for (size_t i = 0; i < sizeof(example1); ++i)
    {
        printf("%c |", ptr1[i]);
    }
    printf("\n         ");
    printStructContents(&example1, sizeof(example1));
    printf("\n\n");

    printf("Size of Example2: %zu", sizeof(example2));
    printf("\nExample2: ");
    for (size_t i = 0; i < sizeof(example2); ++i)
    {
        printf("%c |", ptr2[i]);
    }
    printf("\n         ");
    printStructContents(&example2, sizeof(example2));
    printf("\n\n");

    printf("Size of Example3: %zu", sizeof(example3));
    printf("\nExample3: ");
    for (size_t i = 0; i < sizeof(example3); ++i)
    {
        printf("%c |", ptr3[i]);
    }
    printf("\n         ");
    printStructContents(&example3, sizeof(example3));
    printf("\n");

    printf("\n");

    return 0;
}