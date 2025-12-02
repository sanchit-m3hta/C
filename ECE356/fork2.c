#include <stdio.h>
#include <unistd.h>

int main(void) {
    int id = fork();

    for (int i = 0; i < 2000; i++) {   
        if (id == 0) {
            printf("Child: %d\n", i);
        } else {
            printf("Parent: %d\n", i);
        }
    } 

    return 0;
}
