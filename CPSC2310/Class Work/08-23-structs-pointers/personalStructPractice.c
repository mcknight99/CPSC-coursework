#include <stdio.h>
#include <stdlib.h>

struct StructA {
    int num[5];
    char str[50];
};

int main() {
    // Which is the proper way to initialize the StructA?
    struct StructA a;
    //StructA a;
    //typedef StructA a;
    typedef struct StructA b;

    b c;
    a.num[0]=1;
    a.str = c_str("abcd");
    c.num[0]=2;
}