#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
    /* Write to stdout (usually maps to write syscall) */
    write(STDOUT_FILENO, "starting test\n", 14);
    // 1

    // Create and open a file
    int fd = open("tracer_test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    // 2
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Write some data to the file
    const char *msg = "hello from test_tracer\n";
    write(fd, msg, strlen(msg));
    // 3

    // Close the file
    close(fd);
    // 4

    // Reopen the file for reading
    fd = open("tracer_test.txt", O_RDONLY);
    // 5
    if (fd < 0) {
        perror("open (read)");
        return 1;
    }

    // Read the data back
    char buf[64];
    read(fd, buf, sizeof(buf));
    // 6
    close(fd);
    // 7

    // Allocate and free some memory
    char *mem = malloc(128);
    // 8
    if (!mem) {
        perror("malloc");
        return 1;
    }

    // Use the allocated memory
    strcpy(mem, "testing memory allocation");
    // 9
    free(mem);
    // 10

    // Sleep for a short duration
    usleep(10000);
    // 11

    for(int i = 0; i < 10000; i++) {
        if(i%2==0) {
            usleep(1);
        }
        if(i%5==0) {
            write(STDOUT_FILENO, ".", 1);
        }
        if(i%100==0) {
            int* arr = malloc(10 * sizeof(int));
            free(arr);
        }
    }


    // more rigorous testing:
    for (int i = 0; i < 100; i++) {
        char *temp = malloc(256);
        if (!temp) {
            perror("malloc in loop");
            return 1;
        }
        memset(temp, i, 256);
        // 12 - 111
        free(temp);
        // 112 - 211
    }

    // delete the test file
    unlink("tracer_test.txt");
    // 212

    // fork a child process
    pid_t child_pid = fork();
    // 213
    if (child_pid == 0) {
        // Child process: just exit
        exit(0);
        // 214?
    } else if (child_pid > 0) {
        // Parent process: wait for child to exit
        waitpid(child_pid, NULL, 0);
        // 214?
    } else {
        perror("fork");
        return 1;
    }

    // print the args passed to this program
    if(argc > 1) {
        write(STDOUT_FILENO, "\tArgs passed to test_tracer:\n", 29);  
        // 215
    }
    for (int i = 1; i < argc; i++) {
        write(STDOUT_FILENO, "\t", 1);
        write(STDOUT_FILENO, argv[i], strlen(argv[i]));
        write(STDOUT_FILENO, "\n", 1);
        // 216 - 216 + (argc - 1)
    }


    // Exit the program
    write(STDOUT_FILENO, "ending test\n", 12);
    // 217 + (argc - 1)

    return 0;
}
