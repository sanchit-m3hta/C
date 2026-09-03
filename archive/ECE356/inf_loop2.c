#include <stdio.h>
#include <unistd.h>
int main() {
    while (1) {
    printf("Process is running. PPID: %d\n", getppid());
    sleep(5); // Sleep for 5 seconds to keep the process alive
    }
    return 0;
}
//output: PPID: 76693