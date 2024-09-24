#include <stdio.h>
#include <stdlib.h>

struct Person {
    char name[50];
    int age;
};

int main() {
    struct Person *ptr;
    int i,n;
    printf("Enter the number of persons: ");
    scanf("%d", &n);
    ptr = (struct Person*) malloc(n * sizeof(struct Person));
    for(i = 0; i < n; i++) {
        printf("Enter the name and age of person %d: ", i+1);
        scanf("%s %d", (ptr+i)->name, &(ptr+i)->age);
        // (ptr+i)->name is the same as *(ptr+i).name is the same as ptr[i].name
    }
    printf("Displaying Information:\n");
    for(i = 0; i < n; i++) {
        printf("Name: %s\tAge: %d\n", (ptr+i)->name, (ptr+i)->age);
    }
    free(ptr);
    return 0;   

}