#include <stdio.h>
#define PI 3.14f
int main(void){

    float rad = 10.0f;
    printf("The volume of a sphere with radius %.2fm = %.2fm^3\n", rad, (4.0f/3.0f)*PI*rad*rad*rad);

    return 0;
}