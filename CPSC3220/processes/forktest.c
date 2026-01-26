#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int global_var = 5;

int main() {
    printf("before fork: global_var = %d\n", global_var);
    pid_t p = fork(); 
    if (p==0) {
        global_var = 7;
        printf("im a child: global_var = %d\n", global_var);
    } else {
        //kill(p, SIGKILL);
        wait(NULL);
        //waitpid(p, NULL, 0);
        global_var = 11;
        printf("im a parent: global_var = %d\n", global_var);
    }
    printf("after fork: global_var = %d\n", global_var);
    return 0;
}