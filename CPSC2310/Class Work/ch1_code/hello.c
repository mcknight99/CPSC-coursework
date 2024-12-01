#define NUM 10
//#include <stdio.h>
//#include <stdlib.h>
int printf(const char* restrict, ...) __attribute__((__format__ (__printf__, 1, 2)));

int main() {
    int i = NUM;
    while(i>0) {
        printf("%d = number hello world\n", i);
        i--;
    }
    return 0;
}