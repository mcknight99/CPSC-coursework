#include <stdio.h>

void f(int* p, int* q) {
    printf("inside f ; p is pointing to this address %p\n", p);
    printf("inside f ; q is pointing to this address %p\n", q);
    p = q; //p is now pointing to the same thing q is pointing to
    printf("inside f and after p = q ; p is pointing to this address %p\n", p);
    *p = 2;

}

int main() {
    int i = 0, j = 1;
    int* ipointer = &i;
    int* jpointer = &j;
    printf("Before calling f - ipointer is pointing to this address %p\n", ipointer);
    f(ipointer, jpointer);
    printf("After calling f - ipointer is pointing to this address %p\n", ipointer);
    printf("%d %d and %d %d\n", i, j, *ipointer, *jpointer);
}