//piping3.C
#include <stdio.h>
#include<sys/types.h>
#include<sys/unistd.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/stat.h>

int main(void){
    int pipe1;
    char* myfifo="/tmp/myfifo1";
    pipe1=mkfifo(myfifo,0644);

    printf("%d\n", pipe1);
    printf("named pipe created\n");
    return 0;
}
