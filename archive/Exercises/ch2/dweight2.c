#include <stdio.h>
#define INCHES_PER_POUND 166
#define RECIPROCAL_OF_PI (1.0f/3.14f)
int main(void){

    int height, length, width, volume, weight;

    printf("Enter height: ");
    scanf("%d", &height);
    printf("Enter length: ");
    scanf("%d", &length);
    printf("Enter width: ");
    scanf("%d", &width);

    volume = height*length*width;
    weight = (volume+165)/166;

    printf("Volume: %d\n", volume);
    printf("Dimensional weight: %d\n", weight);
    printf("Reciprocal of pi is %f\n", RECIPROCAL_OF_PI);


    return 0;
}