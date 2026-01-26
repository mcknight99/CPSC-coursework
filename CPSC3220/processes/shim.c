#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int rand(void) {
    int (*original_rand)(void) = dlsym(RTLD_NEXT, "rand"); // dynamically linked symbol // dlsym needs -ldl
    return original_rand() % 100;
    //return 5; // compile as a shared object file (.so) 
    // gcc -Wall -g -shared -fPIC -o shim.so shim.c -ldl
}

// now if you run shim with LD_PRELOAD=./shim.so, rand() will always return 5
// e.g., LD_PRELOAD=./shim.so ./a.out
