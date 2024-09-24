#include <stdio.h>

int f(int, int *, int **);

int main()
{
    int c, *b, **a;
    c = 15;
    b = &c;
    a = &b;

    printf("%d \n", f(c, b, a));
    return 0;
}

int f(int x, int *py, int **ppz)
{
    int y, z;
    (*(*ppz)) += 1;
    z = **ppz;
    *py += 5;
    y = *py;
    x += 2;
    return x + y + z;
}

// what malloc does is it allocates memory on the heap.
// what calloc does is it allocates memory on the heap and initializes it to 0.