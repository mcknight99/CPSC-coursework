#include <stdio.h>
#include <stddef.h>

#define MAX_FIELDS 10

// Helper macro to add a field's offset and size to the arrays
#define ADD_FIELD(struct_type, member)  do { \
    field_offsets[num_fields] = offsetof(struct_type, member); \
    field_sizes[num_fields] = sizeof(((struct_type*)0)->member); \
    num_fields++; \
} while(0)

// Generic function to print the memory layout of a struct
void print_memory_layout(const char* struct_name, size_t struct_size, size_t num_fields, size_t field_offsets[], size_t field_sizes[]) {
    printf("Struct: %s\n", struct_name);
    
    size_t largest_member = 1;
    for (size_t i = 0; i < num_fields; ++i) {
        if (field_sizes[i] > largest_member) {
            largest_member = field_sizes[i];
        }
    }
    
    size_t current_offset = 0;
    while (current_offset < struct_size) {
        printf("[");
        for (size_t i = 0; i < largest_member; ++i) {
            if (current_offset < struct_size) {
                // Check if the current offset corresponds to a field
                int printed = 0;
                for (size_t j = 0; j < num_fields; ++j) {
                    if (current_offset == field_offsets[j]) {
                        printf("%zu", field_sizes[j]);
                        current_offset += field_sizes[j];
                        i += field_sizes[j] - 1;
                        printed = 1;
                        break;
                    }
                }
                if (!printed) {
                    printf("_");
                    current_offset++;
                }
            } else {
                printf("_");
            }
        }
        printf("]");
    }
    
    printf(" = %zu bytes\n\n", struct_size);
}

// Function to analyze a struct and print its memory layout
#define ANALYZE_STRUCT(struct_type, ...)  do { \
    size_t field_offsets[MAX_FIELDS]; \
    size_t field_sizes[MAX_FIELDS]; \
    size_t num_fields = 0; \
    __VA_ARGS__; \
    print_memory_layout(#struct_type, sizeof(struct_type), num_fields, field_offsets, field_sizes); \
} while(0)

// Example structs

//Struct: struct Example1
//[1___][4][1_2] = 12 bytes
struct Example1
{
    char a;
    int c;
    char b;
    short int d;
};

//Struct: struct Example2
//[1___][4][21_] = 12 bytes
struct Example2
{
    char a;
    int b;
    short int c;
    char d;
};

//Struct: struct Example3
//[4][211] = 8 bytes
struct Example3
{
    int a;
    short int b;
    char c;
    char d;
};

int main() {
    ANALYZE_STRUCT(struct Example1,
        ADD_FIELD(struct Example1, a);
        ADD_FIELD(struct Example1, b);
        ADD_FIELD(struct Example1, c);
        ADD_FIELD(struct Example1, d);
    );

    ANALYZE_STRUCT(struct Example2,
        ADD_FIELD(struct Example2, a);
        ADD_FIELD(struct Example2, b);
        ADD_FIELD(struct Example2, c);
        ADD_FIELD(struct Example2, d);
    );

    ANALYZE_STRUCT(struct Example3,
        ADD_FIELD(struct Example3, a);
        ADD_FIELD(struct Example3, b);
        ADD_FIELD(struct Example3, c);
        ADD_FIELD(struct Example3, d);
    );

    return 0;
}
