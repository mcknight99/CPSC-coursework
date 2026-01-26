// print N NUMBER_OF_NUMBERS

#include <stdio.h>
#include <unistd.h>

// int main argc char**argv, if argc>1 srand atoi argv1, then print N random numbers

#define NUMBER_OF_NUMBERS 10

int main(int argc, char** argv) {
    if(argc>1) {
        srand(atoi(argv[1]));
    }

    // print NUMBER_OF_NUMBERS random numbers
    for(int i=0; i<NUMBER_OF_NUMBERS; i++) {
        printf("%d\n", rand());
    }

    return 0;
}

