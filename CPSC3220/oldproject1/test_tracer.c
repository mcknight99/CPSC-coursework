#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#define solotest 2

int main(int argc, char *argv[])
{
    // easy way to differentiate syscalls in the tracer output:
    // repeat each syscall the amount of times equal to the number in the comment
    // syscall functions included in the test:

    // quick test for using argv info
    // print all argv info and run some syscalls based on that
    // e.g. for every int passed in, run "write" that many times
    for (int arg_index = 1; arg_index < argc; arg_index++)
    {
        int count = atoi(argv[arg_index]);
        for (int i = 0; i < count; i++)
        {
            write(STDOUT_FILENO, "arg test", 9);
        }
    }

    // 100 write //verified
    for (int i = 0; i < 10 && solotest == 1; i++)
    {
        write(STDOUT_FILENO, ".", 1);
    }

    // 2 open //verified
    for (int i = 0; i < 20 && solotest == 2; i++)
    {
        int fd = open("tracer_test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd < 0)
        {
            perror("open");
            return 1;
        }
    }

    // 3 write //verified
    const char *msg = "_";
    for (int i = 0; i < 300 && solotest == 3; i++)
    {
        write(STDOUT_FILENO, msg, strlen(msg));
    }

    // 4 close //verified
    for (int i = 0; i < 400 && solotest == 4; i++)
    {
        int fd = open("tracer_test.txt", O_WRONLY);
        if (fd < 0)
        {
            perror("open for close");
            return 1;
        }
        close(fd);
    }

    // 600 read //verified
    char buf[64];
    for (int i = 0; i < 600 && solotest == 6; i++)
    {
        int fd = open("tracer_test.txt", O_RDONLY);
        if (fd < 0)
        {
            perror("open (read)");
            return 1;
        }
        read(fd, buf, sizeof(buf));
        close(fd);
    }

    // 7 close //verified
    for (int i = 0; i < 700 && solotest == 7; i++)
    {
        int fd = open("tracer_test.txt", O_WRONLY);
        if (fd < 0)
        {
            perror("open for close");
            return 1;
        }
        close(fd);
    }

    // 8 malloc // not verified?? is malloc a syscall? answer is no lol
    for (int i = 0; i < 800 && solotest == 8; i++)
    {
        char *mem = malloc(128);
        if (!mem)
        {
            perror("malloc");
            return 1;
        }
        // use the memory to avoid optimization removal
        memset(mem, 0, 128);
        free(mem);
    }

    // 9 strcpy // not verified //answer is no
    for (int i = 0; i < 900 && solotest == 9; i++)
    {
        char *mem = malloc(50);
        if (!mem)
        {
            perror("malloc for strcpy");
            return 1;
        }
        strcpy(mem, "testing memory allocation");
        free(mem);
    }

    // 10 free (using calloc, realloc) // NV; not system call
    for (int i = 0; i < 1000 && solotest == 10; i++)
    {
        char *mem = calloc(1, 256);
        if (!mem)
        {
            perror("calloc");
            return 1;
        }
        mem = realloc(mem, 512);
        if (!mem)
        {
            perror("realloc");
            return 1;
        }
        free(mem);
    }

    // 11 usleep // verified
    // sleep for a short duration
    for (int i = 0; i < 1100 && solotest == 11; i++)
    {
        usleep(1);
    }

    // 12 malloc/free in loop
    // skipping since covered in free and 8

    // 13 unlink // verified
    for (int i = 0; i < 1300 && solotest == 13; i++)
    {
        unlink("tracer_test.txt");
    }

    // 14 fork //verified
    for (int i = 0; i < 1400 && solotest == 14; i++)
    {
        pid_t child_pid = fork();
        if (child_pid == 0)
        {
            // Child process: just exit
            exit(0);
        }
        else if (child_pid > 0)
        {
            // Parent process: wait for child to finish
            waitpid(child_pid, NULL, 0);
        }
        else
        {
            perror("fork");
            return 1;
        }
    }

    // 15 exit/waitpid
    // exit and waitpid also covered in 14 fork above

    // 16 write argv info (already verified write, testing for argument passing; verified)
    for (int i = 0; i < 1600 && solotest == 16; i++)
    {
        write(STDOUT_FILENO, "argv info:\n", 12);
        for (int j = 0; j < argc; j++)
        {
            write(STDOUT_FILENO, argv[j], strlen(argv[j]));
            write(STDOUT_FILENO, "\n", 1);
        }
    }

    // more common & easy syscalls that can be tested:
    // 17 getpid // verified
    for (int i = 0; i < 1700 && solotest == 17; i++)
    {
        pid_t pid = getpid();
        // use pid to avoid optimization removal
        pid = pid;
    }

    // 18 getuid // verified
    for (int i = 0; i < 1800 && solotest == 18; i++)
    {
        uid_t uid = getuid();
        // use uid to avoid optimization removal
        uid = uid;
    }

    // 19 getgid //V
    for (int i = 0; i < 1900 && solotest == 19; i++)
    {
        gid_t gid = getgid();
        // use gid to avoid optimization removal
        gid = gid;
    }

    // 20 getppid //V
    for (int i = 0; i < 2000 && solotest == 20; i++)
    {
        pid_t ppid = getppid();
        // use ppid to avoid optimization removal
        ppid = ppid;
    }

    // 21 getcwd //V
    char cwd[1024];
    for (int i = 0; i < 2100 && solotest == 21; i++)
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            return 1;
        }
    }

    // 23 lseek //V
    for (int i = 0; i < 2300 && solotest == 23; i++)
    {
        int fd = open("tracer_test.txt", O_CREAT | O_RDWR, 0644);
        if (fd < 0)
        {
            perror("open for lseek");
            return 1;
        }
        off_t offset = lseek(fd, 0, SEEK_END);
        if (offset == (off_t)-1)
        {
            perror("lseek");
            close(fd);
            return 1;
        }
        close(fd);
    }

    // execve may be hard to test, could test with forks?
    // execve 25 // V lol very silly but it works
    for (int i = 0; i < 2500 && solotest == 25; i++)
    {
        pid_t child_pid = fork();
        if (child_pid == 0)
        {
            // Child process: execute /bin/ls
            char *args[] = {"/bin/ls", NULL};
            execve("/bin/ls", args, NULL);
            // If execve fails
            perror("execve");
            exit(1);
        }
        else if (child_pid > 0)
        {
            // Parent process: wait for child to finish
            waitpid(child_pid, NULL, 0);
        }
        else
        {
            perror("fork for execve");
            return 1;
        }
    }

    return 0;
}
