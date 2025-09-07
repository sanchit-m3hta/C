#include <stdio.h>

int main(void){
    printf("Enter amount of loan: ");
    float loan = 0.0f;
    scanf("%f", &loan);

    printf("Enter interest rate: ");
    float rate = 0.0f;
    scanf("%f", &rate);
    rate = (rate / 100.00f) / 12;

    printf("Enter monthly payment: ");
    float monthly_payment = 0.0f;
    scanf("%f", &monthly_payment);

    float first_payment = (loan + (loan*rate)) - monthly_payment;
    printf("Balance remaining after first payment: %.2f\n"
        ,first_payment);
    
    float second_payment = (first_payment + (first_payment*rate)) - monthly_payment;
    printf("Balance remaining after second payment: %.2f\n"
        ,second_payment);

    float third_payment = (second_payment + (second_payment*rate)) - monthly_payment;
    printf("Balance remaining after third payment: %.2f\n"
        ,third_payment);

    return 0;
}
