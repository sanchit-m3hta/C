#include <stdio.h>

int main(void){
    /*
    int* p; //p is a pointer to an int
    p = &x; //arrow pointing to x
    */

    int x = 6;
    int y = 10;
    int* p = &x;
    int* q = &y;

    printf("START x=%d, y = %d, *p=%d, *q=%d\n", x,y,*p,*q);

    //task 1: modify p to point at y
    p = q;
    printf("task1 x=%d, y = %d, *p=%d, *q=%d\n", x,y,*p,*q);

    //task 2: create pointer z which points to the pointer q
    int** z = &q;
    //or
    printf("task2 x=%d, y = %d, *p=%d, *q=%d, **z=%d\n", x,y,*p,*q,**z);

    //task 3: assign 1000 to y without using the variable y
    **z = 1000;
    printf("task3 x=%d, y = %d, *p=%d, *q=%d, **z=%d\n", x,y,*p,*q,**z);

    //task4: without using the variable q, make the variable q point a x
    *z = &x;
    printf("task4 x=%d, y = %d, *p=%d, *q=%d, **z=%d\n", x,y,*p,*q,**z);

    //task5 without using x or q, make p point at x
    p = *z;
    printf("task5 x=%d, y = %d, *p=%d, *q=%d, **z=%d\n", x,y,*p,*q,**z);
    return 0;
}