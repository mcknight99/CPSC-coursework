#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

// structure to track each allocation in a linked list
typedef struct allocation
{
    void *ptr;
    size_t size;
    struct allocation *next;
} allocation;

// global head pointer for linked list of allocations
static allocation *alloc_list_head = NULL;

// helper function to add allocation to the list
void add_allocation(void *ptr, size_t size)
{
    // call the real malloc and not our malloc to avoid recursion/just to be safe.
    void *(*real_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    allocation *new_alloc = (allocation *)real_malloc(sizeof(allocation));
    new_alloc->ptr = ptr;
    new_alloc->size = size;
    new_alloc->next = alloc_list_head;
    alloc_list_head = new_alloc;
}

// helper function to remove allocation from the list
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
            // call the real free and not our free to avoid recursion/just to be safe.
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
    if (current == NULL)
    {
        // no leaks detected
        fprintf(stderr, "TOTAL\t0\t0\n"); // prof told me that "0" is a universal non-magic number
        return;
    }
    else
    {
        size_t total_memory_leaked = 0;
        size_t leak_count = 0;
        // converted to do while to avoid magic numbers
        do
        {
            total_memory_leaked += current->size;
            leak_count++;
            current = current->next;
        } while (current != NULL);

        // now print leaks
        current = alloc_list_head;
        while (current != NULL)
        {
            fprintf(stderr, "LEAK\t%zu\n", current->size);
            current = current->next;
        }
        fprintf(stderr, "TOTAL\t%zu\t%zu\n", leak_count, total_memory_leaked);
        return;
    }
}

// this pattern is repeated for each overridden function below
// override -> dlsym and call real function -> track memory -> return result to maintain expected behavior

// malloc is commented to explain the process so they don't have repetitive comments
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
}

void free(void *ptr)
{
    void (*real_free)(void *) = dlsym(RTLD_NEXT, "free");
    real_free(ptr);
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