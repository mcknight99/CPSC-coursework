#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <stdbool.h>
#include <linux/ptrace.h>
#include <string.h> //for memset
#include <signal.h>

#define MAX_SYSCALL_TYPES 1024 // arbitrary large number to cover all syscall numbers. my linux system caps at 372
#define OUTPUT_FILE_POS 2
#define TARGET_PROGRAM_POS 1
#define PTRACE_SYSCALL_BIT 0b10000000 // bit flip magic number added to signal by PTRACE_O_TRACESYSGOOD to indicate syscall stop. using this as a sanity check that it is only stopping for syscalls and not other signals

int main(int argc, char *argv[])
{
    // target_program_with_args is the string passed in as arg1 which contains the target program and its args.
    // it will be parsed into exec_args which is an array of strings to be passed to execvp.
    char *target_program_with_args;
    char *output_file;
    char **exec_args;

    // always treat arg1 as if it has several args to be passed. it will be passed in as a string, so we need to separate it into the target program and its args
    target_program_with_args = argv[TARGET_PROGRAM_POS];
    output_file = argv[OUTPUT_FILE_POS];

    // parse target_program_with_args into exec_args array

    // first count number of args by counting spaces
    int arg_count = TARGET_PROGRAM_POS;
    for (char *p = target_program_with_args; *p != '\0'; p++)
    {
        if (*p == ' ')
        {
            arg_count++;
        }
    }

    // then allocate exec_args array
    exec_args = malloc((arg_count + 1) * sizeof(char *)); // arg_count+1 for NULL terminator
    if (!exec_args)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    // finally split target_program_with_args into exec_args
    int index = 0;
    char *token = strtok(target_program_with_args, " ");
    while (token != NULL)
    {
        exec_args[index++] = token;
        token = strtok(NULL, " ");
    }
    exec_args[index] = NULL; // NULL terminate the array

    unsigned long syscall_counts[MAX_SYSCALL_TYPES];
    memset(syscall_counts, 0, sizeof(syscall_counts)); // initialize all counts to 0 to avoid garbage values

    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process

        // allow the child to be tracked by the parent
        ptrace(PTRACE_TRACEME, NULL);

        // stop the child so the parent can trace the execvp before it runs
        kill(getpid(), SIGSTOP); 

        // pass the target program then the args as an array to execvp
        execvp(exec_args[0], exec_args);

        // since execvp overwrites the process after running, this should never reach here unless execvp fails
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status;
        // wait for child to stop on its first instruction so we can set options and sync up before it makes syscalls or we finish before them
        waitpid(pid, &status, 0);

        // set options to get notified on syscall entry/exit
        ptrace(PTRACE_SETOPTIONS, pid, NULL, PTRACE_O_TRACESYSGOOD);

        while (true)
        {
            // Continue the child and stop at next syscall entry/exit
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);

            // wait for child to stop
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                // break if child has exited or was terminated by a signal
                break;
            }

            // check if there is a syscall stop
            if (WIFSTOPPED(status) && (WSTOPSIG(status) & PTRACE_SYSCALL_BIT))
            {
                struct ptrace_syscall_info info;
                // get syscall info
                ptrace(PTRACE_GET_SYSCALL_INFO, pid, sizeof(info), &info);

                // only count on syscall entry
                if (info.op == PTRACE_SYSCALL_INFO_ENTRY)
                {
                    // increment syscall count for this syscall number
                    unsigned long long syscall_num = info.entry.nr;
                    if (syscall_num < MAX_SYSCALL_TYPES)
                    {
                        syscall_counts[syscall_num]++;
                    }
                }
            }
        }

        // Write results to output file
        FILE *out = fopen(output_file, "w");
        if (!out)
        {
            perror("fopen");
            return EXIT_FAILURE;
        }

        for (int i = 0; i < MAX_SYSCALL_TYPES; i++)
        {
            // only report syscalls that were called at least once
            if (syscall_counts[i] > 0)
            {
                fprintf(out, "%d\t%lu\n", i, syscall_counts[i]);
            }
        }

        fclose(out);
    }

    return EXIT_SUCCESS;
}