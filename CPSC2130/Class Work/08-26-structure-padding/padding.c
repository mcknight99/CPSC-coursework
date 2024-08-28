// structure padding is a concept in C that adds one or more empty bytes between memory addresses to align data in memory

// to minimize the amount of padding, order your struct data members in descending order wrt size (largest to smallest)

#include <stdio.h>
#include <string.h>

// Structs "bunch" by the largest memory usage type, and pad the smallest types if led by larger types
// But if you order by descending size, it allows grouping by pushing padding to the end of the struct

// 4B 4B 1B 1B _B _B 4B
struct structure1 {
    int id1; //4 bytes
    int id2; //4 bytes
    char name; //1 byte
    char c; //1 byte
    float percentage; //4 bytes
};

// 4B 1B 1B _B _B 4B 4B
struct structure2 {
    int id1; //4 bytes
    char name; //1 byte
    char c; //1 byte
    int id2; //4 bytes
    float percentage; //4 bytes
};

//the last two _B allow for the memory to use this extra space
//1B 1B _B _B 4B 4B 2B _B _B
struct structure3 {
    char name; //1 byte
    char c; //1 byte
    int id1; //4 bytes
    int id2; //4 bytes
    short percentage; //2 bytes
};

// 4B 4B 2B 1B 1B _B _B _B _B
// leaves the padded memory at the end
struct betterStruct3 {
    int id1; //4
    int id2; //4
    short percentage; //2
    char name; //1 
    char c; //1
};

//1B 1B 1B 1B 1B
struct structure4 {
    char id1; //1 byte
    char id2; //1 byte
    char name; //1 byte
    char c; //1 byte
    char percentage; //1 byte
};

// [1 _ _ _] [4] [1 _ 2] = 12  // <-- this should be correct in the answer key, as the subdivisions are by the next largest size in the order of the struct
// 4 bytes padding
// [1 _ _ _] [4] [1 2 _] = 12
struct testStruct{
    char a; //1
    int b; //4
    char c; //1
    short int d; //2
};

// [4] [2 1 1] = 8
struct testStruct2{
    int a; //4
    short int b; //2
    char c; //1
    char d; //1
};

//bit fields
typedef struct date {
    // d has value between 1-31, so 5 bits are sufficient to store 0-31
    unsigned int d : 5;
    // what?
    unsigned int : 0;
    // m has value between 1-12 so 4 bits are sufficient to store 0-12
    unsigned int m : 4;
    // largest data size, memory sets aside 4 byte blocks
    unsigned int y;
};

void printMemoryUsage(const void* structPtr, size_t structSize) {
    const unsigned char* bytes = (const unsigned char*)structPtr;
    
    printf("[");
    for (size_t i = 0; i < structSize; i++) {
        printf("%zu ", sizeof(bytes[i]));
    }
    printf("] ");
    
    printf("[");
    for (size_t i = 0; i < structSize; i++) {
        if (i > 0 && sizeof(bytes[i]) > sizeof(bytes[i - 1])) {
            printf("_ ");
        }
        printf("%zu ", sizeof(bytes[i]));
    }
    printf("] = %zu\n", structSize);
}

int main() {
    struct testStruct* a;

    printMemoryUsage(a, sizeof(a));
}