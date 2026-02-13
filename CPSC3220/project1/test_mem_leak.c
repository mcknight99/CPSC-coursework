/*
force a memory leak for the following
Run from the command line
use a shim library and the LD_PRELOAD env var
detect memory leaks
    for this program, a memory leak is defined as a call to malloc, calloc, or realloc that
    returns a pointer that is never freed during execution of the program (either by a call to free or realloc).

when leakcount runs, execute the specified program using fork+exec*, popen, or system
execute the test program with setting LD_PRELOAD to the path to mem_shim.so

*going fork+exec: look at class examples for setenv and putenv. execvpe may be used but is not the only option
*setting LD_PRELOAD env var tells the OS to load shim lib before other libs
    which allows us to replace library calls with our own implementation


example leakcount usage:
    ./leakcount ./my_test_program
    ./leakcount ./test2 ar1 arg2 ...

output to stderr:
LEAK<tab><num bytes>
LEAK<tab><num bytes>
TOTAL<tab><total count><tab><bytes leaked>

example output:
> ./leakcount ./my_test_program
LEAK    128
LEAK    35
TOTAL   2       163
mem_shim.so that will act as a shim to intercept calls to malloc, calloc, realloc, and free

simply, the shim library will be implementing malloc, calloc, realloc, and free
the code should keep track of allocations that occur and which ones get freed
(program should treat realloc like a free then a malloc)

*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // Allocate memory without freeing to simulate a memory leak
    void *leak1 = malloc(128); // Leak 128 bytes and add 1 to the leak count
    void *leak2 = malloc(35);  // Leak 35 bytes and add 1 to the leak count

    // running total: 163 bytes leaked, 2 leaks

    leak1 = leak1; // To avoid unused variable warning
    leak2 = leak2; // To avoid unused variable warning

    // Allocate and free some memory correctly
    void *temp = malloc(64);
    free(temp);
    // should not count as a leak

    // Reallocate memory and free it correctly
    void *temp2 = malloc(50);
    temp2 = realloc(temp2, 100);
    free(temp2);
    // should not count as a leak

    // should be able to handle a LOT of allocations and a LOT of leaks
    int repeat = 10000; //10000
    for (size_t i = 0; i < repeat; i++)
    {
        void *leak = malloc(16); // Leak 16 bytes each iteration
        leak = leak;             // To avoid unused variable warning
    }
    // leak a total of 160000 bytes in addition to the above leaks and add 10000 to the leak count

    // running total: 160163 bytes leaked, 10002 leaks

    // to test realloc leaks
    void *realloc_leak = malloc(20);
    realloc_leak = realloc(realloc_leak, 50); // realloc to a larger size
    // never free realloc_leak to simulate leak
    // this should add 50 bytes to the leak total and 1 to the leak count

    // running total: 160213 bytes leaked, 10003 leaks

    // to test realloc that shrinks and is freed
    void *realloc_no_leak = malloc(30);
    realloc_no_leak = realloc(realloc_no_leak, 10); // realloc to a smaller size
    free(realloc_no_leak);                          // properly free this one
    // should not count as a leak

    // to test realloc that is NULL (acts like malloc)
    void *realloc_null_leak = realloc(NULL, 40); // acts like malloc
    realloc_null_leak = realloc_null_leak;       // To avoid unused variable warning
    // never free realloc_null_leak to simulate leak
    // this should add 40 bytes to the leak total and 1 to the leak count

    // running total: 160253 bytes leaked, 10004 leaks

    // to test realloc that is NULL and then freed
    void *realloc_null_no_leak = realloc(NULL, 60); // acts like malloc
    free(realloc_null_no_leak);                     // properly free this one

    // to test calloc leaks
    void *calloc_leak = calloc(5, 10); // allocate 50 bytes
    calloc_leak = calloc_leak;         // To avoid unused variable warning
    // never free calloc_leak to simulate leak
    // this should add 50 bytes to the leak total and 1 to the leak count

    // running total: 160303 bytes leaked, 10005 leaks

    // to test calloc that is freed
    void *calloc_no_leak = calloc(3, 15); // allocate 45 bytes
    free(calloc_no_leak);                 // properly free this one

    // to test a large number of allocations and leaks with calloc
    repeat = 5000; //5000
    for (size_t i = 0; i < repeat; i++)
    {
        void *leak = calloc(4, 8); // Leak 32 bytes each iteration
        leak = leak;               // To avoid unused variable warning
        // this should be a total of 160000 bytes leaked in addition to the above leaks
    }

    // allocate a leak the size of each arg (args are ints, alloc the value of the int)
    for (int i = 1; i < argc; i++)
    {
        int arg = atoi(argv[i]);
        void *arg_leak = malloc(arg);
        arg_leak = arg_leak; // To avoid unused variable warning
        // never free arg_leak to simulate leak
        // this should add arg_size bytes to the leak total and 1 to the leak count
    }

    // final running total: 320303 bytes leaked, 15005 leaks
    // plus the total size of all command line arguments and an additional argc leaks

    return 0;
}
