#include <stdio.h>

void swap(int, int);
void fixedSwap(int*, int*);

int main() {
    int a = 5;
    int b = 10;
    swap(a, b);
    printf("a: %d, b: %d\n", a, b);
    fixedSwap(&a, &b);
    printf("a: %d, b: %d\n", a, b);
    return 0;
}

void swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

void fixedSwap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}