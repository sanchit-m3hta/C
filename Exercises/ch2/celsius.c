#include <stdio.h>
#define FREEZING_PT 32.0f   // 0c = 32f
#define SCALE_FACTOR (5.0f/9.0f)

int main (void){
    
    float fahrenheit, celsius;
    printf("Enter Fahrenheit Temperature: ");
    scanf("%f", &fahrenheit);
    celsius = (fahrenheit - FREEZING_PT) * SCALE_FACTOR;
    printf("celsius equivalent: %.1f\n", celsius);

    return 0;
}