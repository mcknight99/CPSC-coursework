//sctracer will take a test program and its arguments as a single argument
// then it will run it in a child process and report statistics on the system calls that it uses
// these statistics are reported to an output file specified in the command line

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

int main(int argc, char *argv[]) {
    // simply existing to compile so i can hide the real WIP code 
}