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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // Allocate memory without freeing to simulate a memory leak
    void *leak1 = malloc(128); // Leak 128 bytes
    void *leak2 = malloc(35);  // Leak 35 bytes

    leak1 = leak1; // To avoid unused variable warning
    leak2 = leak2; // To avoid unused variable warning

    // Allocate and free some memory correctly
    void *temp = malloc(64);
    free(temp);

    // Reallocate memory and free it correctly
    void *temp2 = malloc(50);
    temp2 = realloc(temp2, 100);
    free(temp2);

    // Program ends here, leaks should be detected by the shim
    return 0;
}
