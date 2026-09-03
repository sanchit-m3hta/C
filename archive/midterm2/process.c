#include <stdio.h>
#include <unistd.h>

int main(void){
    
    while(1){
        printf("process is running. PID: %d\n", getpid());
        sleep(5);
    }

    return 0;
}