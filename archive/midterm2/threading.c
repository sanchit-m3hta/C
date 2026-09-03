/*Compilation:
gcc threading.c -o threading.o -lpthread
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* routine(){
    printf("hello from threads\n");
    sleep(3);
    printf("ending thread\n");
}

/*pthread_create() returns 0 when it is successful*/

int main(void){

    pthread_t t1, t2;

    if (!(pthread_create(&t1, NULL, &routine, NULL))){
        printf("thread 1 successfully created!\n");
        pthread_join(t1, NULL);
    } else {
        printf("threading 1 failed\n");
    }
     if (!(pthread_create(&t2, NULL, &routine, NULL))){
        printf("thread 2 successfully created!\n");
        pthread_join(t2, NULL);
    }else {
        printf("threading 2 failed\n");
        pthread_join(t2, NULL);
    }


    return 0;
}