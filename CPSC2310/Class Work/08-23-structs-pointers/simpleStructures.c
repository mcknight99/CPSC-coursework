#include <stdio.h>

// A variable declaration like basic data types

typedef struct Point{
    int x;
    int y;
    int* z;
} point_t;

int main() {
    point_t p1;
    printf("sizeof(p1) = %lu\n", sizeof(p1));
    printf("address of p1.x = %p\n", &p1.x);
    printf("address of p1.y = %p\n", &p1.y);
    printf("address of p1.z = %p\n", &p1.z);

    struct Point p2;
    printf("sizeof(p2) = %lu\n", sizeof(p2));
    printf("address of p2.x = %p\n", &p2.x);
    printf("address of p2.y = %p\n", &p2.y);
    printf("address of p2.z = %p\n", &p2.z);

    struct Point* ptr = &p2;
    printf("value of ptr = %p\n", ptr);
    printf("address of ptr = %p\n", &ptr);
    printf("address of ptr->x = %p\n", &ptr->x);
    printf("address of ptr->y = %p\n", &ptr->y);
    printf("address of ptr->z = %p\n", &ptr->z);

    p1.x = 0;
    p1.y = 1;
    printf("p1.x = %d, p1.y = %d\n", p1.x, p1.y);
    p2.x = 2;
    p2.y = 4;
    printf("p2.x = %d, p2.y = %d\n", p2.x, p2.y);
    ptr->y = 5;

    // ptr->z = &ptr->x'; //is the same as the next line
    (*ptr).z = &(*ptr).x;
    printf("p2.x = %d, p2.y = %d, *(*ptr).z = %p\n", p2.x, p2.y, *(*ptr).z);
    printf("p2.x = %d, p2.y = %d, *ptr->z = %p\n", p2.x, p2.y, *ptr->z);

    (*ptr).x=10;
    printf("p2.x = %d, p2.y = %d, *ptr->z = %p\n", p2.x, p2.y, *ptr->z);


}