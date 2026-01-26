#include <stdio.h>
#include <unistd.h>

int main() {

    int x = 7;
    char* args[4] = {"ls", "-al", ".", NULL};

    if(fork() == 0) {
        int i = execvp("ls", args);
        return 0;
    }

    wait(NULL);

    x++;
    printf("after execvp, x = %d\n", x);;

    ptrace(PTRACE_TRACEME);
    kill(getpid(), SIGSTOP);


}