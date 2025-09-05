#include <stdio.h>


int add(int a, int b){
    return a + b;
}

int main(){

    int a,b;
    a = 5, b = 7;

    printf("%d + %d = %d\n", a, b, add(a,b));

    return 0;
}