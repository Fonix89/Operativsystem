#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int tall1 = 10;
int tall2 = 20;
int tabell[10] = {0,1,2,3,4,5,6,7,8,9};

void *start(void *arg) 
{
    int index = *(int *)arg;
    //tall = tall + 1;
    //printf("Starter tråd tall er: %d %lu\n",tall, pthread_self());
    sleep(1);
    printf("Starter tråd tall er på posisjon %d %d \n", index, tabell[index]);
    free(arg);
    //printf("Slutter tråd tall er: %d %lu\n",tall, pthread_self());
    return 0;   
}

int main(int argc, char *argv[]) {
    pthread_t traad[10];
    for(int i=0; i < 10; i++) {
        int* a = malloc(sizeof(int));
        *a = i;

        if(pthread_create(&traad[i], NULL, &start, &i) != 0) {
            return 1;
        }
    }

    pthread_t traad1; // Declare the variable traad1
    for(int i=0; i < 10; i++) {
        if(pthread_join(traad[i], NULL) != 0) {
            return 2;
        }
    }

    return 0;
}