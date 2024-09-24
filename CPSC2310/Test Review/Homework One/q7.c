#include <stdio.h>
struct date {
int d : 5;
int m : 4;
int y;
};

int main()
{
struct date dt = { 31, 12, 2020 };
printf("Date is %d/%d/%d\n", dt.d, dt.m, dt.y);
printf("%lu", sizeof(struct date));
return 0;
}

// Output: Date is -1/-4/2020
// 8

// Describe why it does not work and how to fix it
// The date struct is not working because the bit fields are not large enough to hold the values that are being assigned to them. 
// The d field is 5 bits, the m field is 4 bits. The d field is not large enough to hold the value 31, so it is being truncated to -1. The m field is not large enough to hold the value 12, so it is being truncated to -4. 
//The y field is large enough to hold the value 2020. To fix this, the d field should be 6 bits and the m field should be 5 bits. This will allow the fields to hold the values that are being assigned to them.
// struct date {
// int d : 6;
// int m : 5;
// int y;
// };

