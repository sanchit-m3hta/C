#include <stdio.h>
#define TAX 0.05f

int main(void){

    printf("Please enter an amount: ");
    float amount = 0;
    scanf("%f", &amount);
    printf("Amount with 5%% tax = %.2f\n", amount + amount*TAX);

    return 0;
}