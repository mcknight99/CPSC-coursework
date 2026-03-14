// condition variables example in C using pthreads
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

pthread_mutex_t mutex;
pthread_cond_t cond_var;
int ready = 0;


// i should start to try and use more asserts for debugging/sanity checking







// use #ifdef BUSY #else #endif 