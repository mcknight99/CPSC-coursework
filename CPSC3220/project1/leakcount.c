/*
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

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>