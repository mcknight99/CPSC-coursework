// QUESTION ABOUT OUTPUT TO FILE: should the last row have a newline? (leave a blank line at the end?)

// sctracer will take a test program and its arguments as a single argument
//  then it will run it in a child process and report statistics on the system calls that it uses
//  these statistics are reported to an output file specified in the command line

// we want to watch all system calls rather than a few
// so ptrace can be used instead of a shim for every call
// ptrace can read or write to another process's memory or registers, monitor signals received by the process, stop and start the process when a syscall occurs, and kill the process
// this project only uses PTRACE_TRACEME, PTRACE_SYSCALL, PTRACE_PEEKUSER, PTRACE_SETOPTIONS, and PTRACE_O_TRACESYSGOOD
// read $man ptrace$ for crucial info
// read up on waitpid, getpid, and other process management functions

// use fork to create new process. child should call ptrace(PTRACE_TRACEME) before calling exec*

// once the child is complete, the program should write results to output in format:
/*
<syscall number><tab><num calls>
<syscall number><tab><num calls>
...
*/
// results should be ordered by system call number in increasing order (smallest first)
// do not report entries for syscalls that were never called/have 0 calls

// he just emailed that we should use PTRACE_GET_SYSCALL_INFO instead of PEEKUSER to get syscall numbers to tell the difference between entry and exit easier

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
// #include <sys/user.h>
// #include <stdint.h>
#include <stdbool.h>
#include <linux/ptrace.h> // for ptrace_syscall_info
#include <string.h>       //for memset
#include <signal.h>      // for sigstop

// overall system flow:
// child: ptrace(PTRACE_TRACEME) -> execvp(target program)
// parent: waitpid -> ptrace(PTRACE_SETOPTIONS) to set PTRACE_O_TRACESYSGOOD
// loop: waitpid -> ptrace(PTRACE_GETREGS) to get syscall number -> ptrace(PTRACE_SYSCALL) to continue child
// on exit: write results to output file

#define MAX_SYSCALL_TYPES 1024              // arbitrary large number to cover all syscall numbers. haven't seen one yet over 350. larger this gets, slower and less efficient and reliable the memory gets
#define OUTPUT_FILE_ARG_POSITION (argc - 1) // last argument is output file
#define MIN_ARGS 3                          // at least program name, target program, output file

int main(int argc, char *argv[])
{
    // min args:
    // ./sctracer ./test out.txt
    // passing args to test program:
    // ./sctracer “./test arg1 arg2 argF” out.txt

    if (argc < MIN_ARGS)
    {
        fprintf(stderr, "Usage: %s <target_program> [args ...] <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *target_program_with_args;
    char *output_file;
    char **exec_args;

    // always treat arg1 as if it has several args to be passed. it will be passed in as a string, so we need to separate it into the target program and its args
    // e.g. "./test_tracer 2 3" -> output_file = "./test_tracer", exec_args = {"./test_tracer", "2", "3", NULL}
    target_program_with_args = argv[1];
    output_file = argv[2];
    // parse target_program_with_args into exec_args array
    // first count number of args by counting spaces
    int arg_count = 1; // at least one arg //?? i assume this is a magic number lol
    for (char *p = target_program_with_args; *p != '\0'; p++)
    {
        if (*p == ' ')
        {
            arg_count++;
        }
    }
    // allocate exec_args array
    exec_args = malloc((arg_count + 1) * sizeof(char *)); // +1 for NULL terminator
    if (!exec_args)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }
    // split target_program_with_args into exec_args
    int index = 0;
    char *token = strtok(target_program_with_args, " ");
    while (token != NULL)
    {
        exec_args[index++] = token;
        token = strtok(NULL, " ");
    }
    exec_args[index] = NULL; // NULL terminate the array

    // old processing
    // const char *output_file = argv[OUTPUT_FILE_ARG_POSITION];
    // char **exec_args = &argv[1];  //?? magic number? can i just keep #defining magic numbers and make it easy on myself?
    // argv[OUTPUT_FILE_ARG_POSITION] = NULL; // exclude output file from execvp args, this should work since it acts as a null terminator for execvp

    unsigned long syscall_counts[MAX_SYSCALL_TYPES];   // = {0}; //doesnt make a difference //?? feels like magic numbers
    memset(syscall_counts, 0, sizeof(syscall_counts)); //?? feels like magic numbers but necessary to avoid garbage values

    pid_t pid = fork();

    if (pid == 0) //?? magic number?
    {
        // Child process
        // allow the child to be tracked by the parent
        ptrace(PTRACE_TRACEME, NULL, NULL, NULL); // used to be NULL, NULL, NULL but seems to work with just NULL, but the ptrace_syscall needs two nulls?, odd still quite confused
        // stop the child so the parent can trace the execvp before it runs
        kill(getpid(), SIGSTOP); // comes from https://github.com/sorber/cpscece3220-s26/blob/main/processes/ptrace_example.c
        
        execvp(exec_args[0], exec_args);          //?? does that count as a magic number?
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status;
        // wait for child to stop on its first instruction so we can set options and sync up before it makes syscalls or we finish before them
        waitpid(pid, &status, 0); // wants to be 0 instead of null for -Wall??

        // set options to get notified on syscall entry/exit
        ptrace(PTRACE_SETOPTIONS, pid, NULL, PTRACE_O_TRACESYSGOOD);

        while (true)
        {
            // Continue the child and stop at next syscall entry/exit
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            // wait for child to stop
            waitpid(pid, &status, 0); // wants to be 0 instead of null for -Wall //?? magic numbers?
            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                // break if child has exited or was terminated by a signal
                break;
            }

            // check if there is a syscall stop
            if (WIFSTOPPED(status) && (WSTOPSIG(status) & 0b10000000)) //?? magic number most likely lol what to do about it
            // (0x80/0b10000000 is magic number added by PTRACE_O_TRACESYSGOOD) and flips the 7th bit to 1 to indicate syscall stop
            // https://www.man7.org/linux/man-pages/man2/ptrace.2.html -> PTRACE_O_TRACESYSGOOD section
            {
                // print WSTOPSIG(status); //debugging line to see stop signals
                // printf("Stop signal: %d\n", WSTOPSIG(status));
                // printed as binary:
                // printf("\nStop signal (binary): ");
                // for (int i = 7; i >= 0; i--) {
                //     printf("%d", (WSTOPSIG(status) >> i) & 1);
                // }
                // printf("\n");

                // Child has stopped due to a syscall entry/exit
                // get syscall info
                struct ptrace_syscall_info info;
                // memset(&info, 0, sizeof(info)); // initialize to zero to avoid garbage values // well it has no effect with or without it in contrast to syscall_counts but i think that makes sense since it is being fully overwritten by ptrace right after?
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
            if (syscall_counts[i] > 0) // only report syscalls that were called
            {
                fprintf(out, "%d\t%lu\n", i, syscall_counts[i]);
            }
        }

        fclose(out);
    }

    return EXIT_SUCCESS;
}