#include <stdio.h>
//add.o is relocated the ram
//bringing a program into the RAM is called loading
//add is just floating around ig?

int main(void){
    int a, b;
    printf("enter a: ");
    scanf("%d", &a);
    printf("enter b: ");
    scanf("%d", &b);
    printf("%d\n", a + b);

}