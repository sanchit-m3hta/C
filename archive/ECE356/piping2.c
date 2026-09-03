#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//int main (void)
void main(void)
{
    int id, fd[2], num;
    pipe(fd);
    id = fork();
    if (id ==0){
        printf("Enter number: ");
        scanf("%d", &num);
        write(fd[1], &num, sizeof(int));
    } else {
        read(fd[0], &num, sizeof(int));
        if (num % 2== 0){
            printf("\nOutput from child: %d is even \n", num);
        } else{
            printf("Output from child: %d is odd \n", num);
        }
    }
    //return 0;

}