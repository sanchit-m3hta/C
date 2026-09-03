#include <stdio.h>

int main(void){
    
    printf("Please enter a value for x: ");
    float x;
    scanf("%f", &x);
    float value = (3*x*x*x*x*x) + (2*x*x*x*x) - (5*x*x*x) - (x*x) + (7*x) - 6;
    printf("the value of the polynomial '3x^5 + 2x^4 - 5x^3 -x^2 + 7x - 6' is %.2f\n", value);

    return 0;
}