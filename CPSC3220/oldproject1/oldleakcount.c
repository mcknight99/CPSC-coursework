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

#define MIN_ARGS 2 // at least needs this program + target program

int main(int argc, char *argv[])
{
    // ./leakcount ./test arg1 arg2 argF
    // if (argc < MIN_ARGS) {
    // fprintf(stderr, "Usage: %s <program> [args...]\n", argv[0]);
    //     return 1; //EXIT_FAILURE
    // }

    // Prepare arguments for exec
    char **exec_args = &argv[1];

    // Fork a child process
    pid_t pid = fork();

    if (pid == 0)
    { //?? magic number?
        // Child process
        // Set LD_PRELOAD to the path of mem_shim.so
        setenv("LD_PRELOAD", "./mem_shim.so", 1); //?? magic number?

        // Execute the target program
        execvp(exec_args[0], exec_args);

        // If execvp returns, an error occurred
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
}