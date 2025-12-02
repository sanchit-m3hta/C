#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//int main (void)
void main(void)
{
    int id, fd[2], num;
    char buffer[7] = "hello\n";
    pipe(fd);
    id = fork();
    if (id ==0){
        printf("Sending %s\n", buffer);
        //scanf("%d", &num);
        write(fd[1], buffer, 7);
    } else {
        read(fd[0], buffer, 7);
        printf("\nOutput from child: %s\n", buffer);
    }
    //return 0;

}