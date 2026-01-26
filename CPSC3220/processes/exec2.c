#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(int arc, char**argv) {
    int x = 7;
    pid_t p;

    if((p=fork())==0) {
        execlp("ls", "ls", "-al", NULL);
        return 0;
    }
    wait(NULL);
    x++;
    printf("after execl, x = %d\n", x);;
    // now the 8 will show up after running a program
}