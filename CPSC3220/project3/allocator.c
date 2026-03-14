#include <fcntl.h> 
#include <stdlib.h> 
#include <sys/mman.h> 
#include <string.h> 

#define PAGESIZE 4096 

int main() {
    // you can request a page like this. 
    void * page = mmap (NULL, PAGESIZE, 
                        PROT_READ | PROT_WRITE, 
                        MAP_PRIVATE | MAP_ANONYMOUS,-1, 0); 

    // And, you can unmap the page, when you’re done. 
    munmap(page, PAGESIZE);
    return 0;
}