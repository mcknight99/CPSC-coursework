#include <stdio.h>

int main()
{
int arr[ 10 ] = {10,19,45,18,13,98,200,33,99,1};
int * ptr = arr;
printf("value of what ptr is pointing to: %d\n", *ptr);  
int* ptr2 = ptr+5;
printf("value of what ptr2+5 is pointing to: %d\n", *(ptr2));
printf("value of what (ptr + 5) +2 is pointing to: %d\n", *(ptr+5)+2);

return 0;
}