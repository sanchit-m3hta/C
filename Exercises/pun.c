#include <stdio.h>
/*To Compile:
*gcc -o pun.o pun.c
*/

int main(void){
    //
    printf("To C, or not to c: that is the question.\n");

    int age;
    age = 22;
    //printf("%.1f\n", age);

    float weight = 72.50f;
    //printf("weight: %.2f", weight);

    printf("age: %d\nheight: %.2f\n", age, weight);


    return 0;
}
