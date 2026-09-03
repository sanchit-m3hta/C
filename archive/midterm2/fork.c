#include <stdio.h>
#include <unistd.h>

int main(void){

    int num = fork();

    if (num == 0){
        printf("Child: Hi parent! Can I get some ice cream\n");
        sleep(1);
        printf("Are you there, Parent?\n");
    } else if (num != 0 && num != 0) {
        sleep(2);
        printf("Yes, child, after you finish you hw\n");
    }


    return 0;
}