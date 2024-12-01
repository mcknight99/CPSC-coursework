#include <stdio.h>

int main()
{
    int iArr[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    int *iPtr = iArr;

    printf("%d\n", iPtr[0]);
    printf("%d\n", iPtr[0] + 1);
    printf("%d\n", *(iPtr + 5));
    printf("%d\n", *(iPtr + 3) + 5);
    printf("%d\n", *(iPtr + 2));
    return 0;
}