// sctracer will take a test program and its arguments as a single argument
//  then it will run it in a child process and report statistics on the system calls that it uses
//  these statistics are reported to an output file specified in the command line

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
/*
mem_shim.so that will act as a shim to intercept calls to malloc, calloc, realloc, and free

simply, the shim library will be implementing malloc, calloc, realloc, and free
the code should keep track of allocations that occur and which ones get freed
(program should treat realloc like a free then a malloc)

*/

#include <stdio.h>
#include <stdlib.h>
// #include <pthread.h> // might be needed for thread safety if something doesnt work. flags are already set in makefile. everything has worked without it so far
#include <dlfcn.h>

// memory leak detector hints for implementation
// void free(void *ptr);
// void *malloc(size_t size);
// void *calloc(size_t count, size_t size);
// void *realloc(void *ptr, size_t size);

// how to call the original version of malloc hint
// #include <dlfcn.h>
// void *(*real_malloc)(size_t size);
// real_malloc = dlsym(RTLD_NEXT, "malloc");
// void *ptr = real_malloc(17); //why 17?

typedef struct allocation
{
    void *ptr;
    size_t size;
    struct allocation *next;
} allocation;

// global head pointer for linked list of allocations
static allocation *alloc_list_head = NULL;

// i found it a pain to manage the linked list inside each function so these will manage adding/removing allocations
void add_allocation(void *ptr, size_t size)
{
    // call the real malloc and not our malloc
    // i'm like 80% certain this is required here since the first match to malloc would be the shim so i think (?) i need to use dlsym to avoid recursion
    void *(*real_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    allocation *new_alloc = (allocation *)real_malloc(sizeof(allocation));
    new_alloc->ptr = ptr;
    new_alloc->size = size;
    new_alloc->next = alloc_list_head;
    alloc_list_head = new_alloc;
}

void remove_allocation(void *ptr)
{
    allocation *current = alloc_list_head;
    allocation *previous = NULL;
    while (current != NULL)
    {
        if (current->ptr == ptr)
        {
            if (previous == NULL)
            {
                alloc_list_head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            // call the real free and not our free
            void (*real_free)(void *) = dlsym(RTLD_NEXT, "free");
            real_free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

// print leak report at program exit
__attribute__((destructor)) void report_leaks()
{
    allocation *current = alloc_list_head;
    // size_t total_leaked = 0; //?? is this a magic number and how could i get rid of it
    // size_t leak_count = 0; //?? i feel like i can do the following:
    // if the current (alloc_list_head) is not null, i can start by making total_leaked = current->size and leak_count = 1
    // then i can just loop through the rest of the list adding sizes and incrementing count
    if (current == NULL)
    {
        // no leaks detected
        fprintf(stderr, "TOTAL\t0\t0\n"); // still feels like magic numbers
        return;
    }
    else
    {
        size_t total_leaked = current->size;
        size_t leak_count = 1; // still feels like magic numbers
        current = current->next;
        while (current != NULL)
        {
            total_leaked += current->size;
            leak_count++;
            current = current->next;
        }
        // now print leaks
        current = alloc_list_head;
        while (current != NULL)
        {
            fprintf(stderr, "LEAK\t%zu\n", current->size);
            current = current->next;
        }
        fprintf(stderr, "TOTAL\t%zu\t%zu\n", leak_count, total_leaked);
        return;
    }
    // below is commented out to keep original work from "magic numbers" with zeros
    // while (current != NULL) {
    //     fprintf(stderr, "LEAK\t%zu\n", current->size);
    //     total_leaked += current->size;
    //     leak_count++;
    //     current = current->next;
    // }
    // fprintf(stderr, "TOTAL\t%zu\t%zu\n", leak_count, total_leaked);
}

// basic implementation structure of the shim:
// overridden function name and parameters to match as the "first" implementation for the compiler
// inside the function, use dlsym to get the "real" version of the function
// call the real version and track the allocation/deallocation
// return the result from the real version to maintain expected behavior
// note: never use my overridden functions or there will be recursion

void *malloc(size_t size)
{
    // match real malloc and use dlsym to get the real version
    void *(*real_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    // call the real malloc
    void *ptr = real_malloc(size);
    // if allocation successful, track it
    if (ptr)
    {
        add_allocation(ptr, size);
    }
    // return to maintain behavior
    return ptr;
    // rinse and repeat with below functions
}

void free(void *ptr)
{
    void (*real_free)(void *) = dlsym(RTLD_NEXT, "free");
    real_free(ptr);
    // remove allocation from allocation list
    if (ptr)
    {
        remove_allocation(ptr);
    }
}

void *calloc(size_t count, size_t size)
{
    void *(*real_calloc)(size_t, size_t) = dlsym(RTLD_NEXT, "calloc");
    void *ptr = real_calloc(count, size);
    if (ptr)
    {
        add_allocation(ptr, count * size);
    }
    return ptr;
}

void *realloc(void *ptr, size_t size)
{
    void *(*real_realloc)(void *, size_t) = dlsym(RTLD_NEXT, "realloc");
    void *new_ptr = real_realloc(ptr, size);
    if (new_ptr)
    {
        // treat realloc as free then malloc
        if (ptr)
        {
            remove_allocation(ptr);
        }
        add_allocation(new_ptr, size);
    }
    return new_ptr;
}