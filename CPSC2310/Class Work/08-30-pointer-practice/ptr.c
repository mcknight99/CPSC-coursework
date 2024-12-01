#include <stdio.h>

int main() {
    /* Can you think of a reason why you might want to have a pointer to an array*/
    int iArr[] = {1,2,3};
    int *iPtr = iArr;
    char cArr[] = {64, 'A', 98}; // 64 = @ // 98 = b // 65 = A
    char *cPtr = cArr;


    printf("cPtr[0] = %c\n", cPtr[0]);
    printf("cArr[0] = %c\n", cArr[0]);
    printf("*cArr = %c\n", *cArr);
    printf("*(cPtr+1) = %c\n", *(cPtr+1));
    printf("*(cArr+1) = %c\n", *(cArr+1));
    printf("*(cPtr+1)+1 = %c\n", *(cPtr+1)+1);
    printf("*(cArr+1)+1 = %c\n", *(cArr+1)+1);
    printf("*(cPtr+2) = %c\n", *(cPtr+2));
    printf("*(cArr+2) = %c\n", *(cArr+2));
    printf("*(cPtr+2)+1 = %c\n", *(cPtr+2)+1);
    printf("*(cArr+2)+1 = %c\n", *(cArr+2)+1);
    

}
