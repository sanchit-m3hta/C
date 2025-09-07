#include <stdio.h>

int main(void){
    
    printf("Enter a dollar amount: ");
    int amount = 0;
    scanf("%d", &amount);
    int twenty = 0, ten = 0, five = 0, one = 0;
    twenty = amount / 20;
    printf("$20 bills: %d\n", twenty);
    ten = (amount - (20*twenty)) / 10 ;
    printf("$10 bills: %d\n", ten);
    five = (amount - (20*twenty) - (10*ten)) / 5;
    printf("$5 bills: %d\n", five);
    one = (amount - (20*twenty) - (10*ten) - (5*five)) / 1;
    printf("$1 bills: %d\n", one);
    
    return 0;
}