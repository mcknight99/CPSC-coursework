/*
mem_shim.so that will act as a shim to intercept calls to malloc, calloc, realloc, and free

simply, the shim library will be implementing malloc, calloc, realloc, and free 
the code should keep track of allocations that occur and which ones get freed
(program should treat realloc like a free then a malloc)

*/

// test change



// memory leak detector hints for implementation
void free(void *ptr);
void *malloc(size_t size);
void *calloc(size_t count, size_t size);
void *realloc(void *ptr, size_t size);

// how to call the original version of malloc
#include <dlfcn.h>
void *(*original_malloc)(size_t size);
original_malloc = dlsym(RTLD_NEXT, "malloc");
void *ptr = original_malloc(17); //why 17?