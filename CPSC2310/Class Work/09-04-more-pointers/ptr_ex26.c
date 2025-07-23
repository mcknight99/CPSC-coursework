#include <stdio.h>

void f(int* , int);

int main() {
    int i = 6, j = 2;
    f(&i, j);
    printf("i = %d j = %d\n", i, j);
    printf("%d\n", i + j);
    return 0;
}

void f(int* p, int m) {
    m = m+8;
    *p = *p + m;
}