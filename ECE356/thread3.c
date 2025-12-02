#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void* routine (){
    for (int i = 0; i < 4; i++){
        printf("%d\n", i);
    }
}
int main(void){
    pthread_t t1;
    pthread_create(&t1, NULL, &routine,  NULL);
    pthread_join(t1, NULL);

    printf("Inside main thread\n");
    for(int i = 24; i > 20; i--){
        printf("%d\n", i);
    }
    printf("Exiting main\n");

    return 0;
}