#include <stdio.h>
#include <stdlib.h>

int main()
{
    int values[10] = {-5, 1, -39, 78, 8, 45, -7, 10, -15, 90};
    char word[38] = {'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 'g', 'o', 'i', 'n', 'g', ' ', 't', 'o', ' ', 'b', 'e', ' ', 'a', ' ', 'g', 'r', 'e', 'a', 't', ' ', 's', 'e', 'm', 'e', 's', 't', 'e', 'r', '!', '\0'};
    int i = 0;

    int* pv0 = values;
    printf("*pv0 = %i\n", *pv0);
    int* pv1 = pv0 + 8;
    printf("*pv1 = %i\n", *pv1);
    char* pv2 = word;
    printf("*pv2 = %c\n", *pv2);
    printf("*(pv2 + 2) = %c\n", *(pv2 + 2));

    char* pv4 = word;
    for (i = 0; i < 6; i++)
    {
        printf("%c", *(pv4));
        pv4 += 3;
    }
    printf("\n%s\n", word);
}