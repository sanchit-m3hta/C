#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int id = fork();

    if (id == 0) {
        // Child process
        for (int i = 0; i < 2000; i++) {
            printf("Child: %d\n", i);
        }
    } else {
        // Parent process waits for child to finish
        wait(NULL);
        for (int i = 0; i < 2000; i++) {
            printf("Parent: %d\n", i);
        }
    }

    return 0;
}
