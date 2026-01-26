#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    while (1) {
        if (fork() == 0) {
            printf("child started and done (%d)\n", getpid());
            return 0;
        } else {
            //wait(NULL);
            printf("parent waiting for child (%d)\n", getpid());
            //waitpid(-1, NULL, 0);
            sleep(5);
        }
    }
}