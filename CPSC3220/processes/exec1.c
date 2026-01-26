#include <unistd.h>
#include <stdio.h>

int main(int arc, char**argv) {
    int x = 7;
    execl("ls", "ls", "-l", NULL); // exec replaces the current loaded code with new code. if exec succeeds, everything that runs after it does not happen because exec returns
    // format: filename, arg0, arg1, ..., argn, NULL
    // ls = argv[0] for the new program
    // -1 = argv[1] for the new program
    // NULL = end of args (terminator)
    perror("[execl failed]");
    x++;
    printf("after execl, x = %d\n", x);;
}