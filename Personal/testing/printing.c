#include <stdio.h>
#include <stdint.h>

int main()

{

    float arr[ 10 ] = {70.5, 0.5, 17.8, 5.9, 19.5, 6.3, 8.2, 5.6, 2.1, 6.4};

    float *ptr1 = &arr[ 1 ];

    float *ptr2 = ptr1 + 6;

 

    printf("%.2f\n", *ptr2); //print 1

    printf("%ld\n", ptr2 - ptr1); // print 2

    
    int8_t x = 22;

    int8_t y = 244;

    printf("%d", (x + y));

   return 0;

}