#include <stdio.h>

int main(void){

    int a = 'A';
    int b = 'B';
    int* p = &a;
    int* q = &b;
    int** x = &p;
    int** y = &q;

    /*Task:
    Using only x and y, make a = C, b = D, *p = D, *q = D
    */
    //set a and b to C and D respectively
    **x = 'C'; 
    **y = 'D';
    //*x = 'D';
    //*y = 'D';
    *x = *y;



    printf("a is '%c', b is '%c'\n", a,b);
    printf("*p is '%c', *q is '%c'\n", *p, *q);

    return 0;
}