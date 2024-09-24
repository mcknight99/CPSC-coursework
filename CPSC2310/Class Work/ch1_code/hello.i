# 0 "hello.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "hello.c"



int printf(const char* restrict, ...) __attribute__((__format__ (__printf__, 1, 2)));

int main() {
    int i = 10;
    while(i>0) {
        printf("%d = number hello world\n", i);
        i--;
    }
    return 0;
}
