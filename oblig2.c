#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int balanse = 0;
int tall = 0;

void *start() {
    tall = tall + 1;
    printf("Starter tråd, med tall %d %lu\n", tall, pthread_self());

    sleep(5);
    printf("Avslutter tråd %d %lu\n", tall, pthread_self());
}

int main(int arch, char *argv[]) {
    pthread_t tråd1, tråd2;

    if(pthread_create(&tråd1, NULL, &start, NULL) != 0) {
        printf("Kunne ikke lage tråd 1\n");
        return 1;
    }

    if(pthread_create(&tråd2, NULL, &start, NULL) != 0) {
        printf("Kunne ikke lage tråd 2\n");
        return 3;
    }

    if(pthread_join(tråd1, NULL) != 0) {
        printf("Kunne ikke hente tråd 1\n");
        return 2;
    }

    if(pthread_join(tråd2, NULL)) {
        printf("Kunne ikke hente tråd 2\n");
        return 4;
    }

    return 0;
}
