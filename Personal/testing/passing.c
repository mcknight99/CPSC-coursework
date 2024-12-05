#include <stdio.h>

int f(int x, int *py, int **ppz)
{

    int y, z, i;

    **ppz += 3;

    z = **ppz;


    for (i = 0; i < (x - 6); i++)

        *py += 4;

    y = *py;

    x += 9;

    return x + y + z;
}

int main()
{

    int c, *b, **a;

    c = 12;

    b = &c;

    a = &b;

    printf("%d\n", f(c, b, a));

    return 0;
}