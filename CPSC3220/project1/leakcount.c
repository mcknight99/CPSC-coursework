#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define EXEC_ARGS_POS 1

int main(int argc, char *argv[])
{
    // Prepare arguments for exec
    char **exec_args = &argv[EXEC_ARGS_POS];

    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process
        // Set LD_PRELOAD to the path of mem_shim.so
        setenv("LD_PRELOAD", "./mem_shim.so", true);

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