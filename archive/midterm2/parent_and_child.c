#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (void) {
    int id = fork();

    if (id == -1) {
        fprintf(stderr, "fork() failed!\n");
        return 1;
    }
    if (id != 0){
        printf("hello from parent with PID = %d, and Parent PID = %d\n", getpid(), getppid());
    } else if (id == 0) {
        printf("hello from child with PID = %d, and Parent PID = %d\n", getpid(), getppid());
    }

    return 0;
}