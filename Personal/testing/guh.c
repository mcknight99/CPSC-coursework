#include <stdio.h>

 

int main()

{

    float arr[10] = {70.5, 0.5, 17.8, 5.9, 19.5, 6.3, 8.2, 5.6, 2.1, 6.4};

    float *ptr1 = &arr[1];

    float *ptr2 = ptr1 + 6;

 

    printf("%.2f\n", *ptr2);

    printf("%ld\n", ptr2 - ptr1);

   

   return 0;

}