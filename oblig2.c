#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int balanse = 0;
//Mutex for å synkronisere tilgang til balanse
pthread_mutex_t balanse_mutex = PTHREAD_MUTEX_INITIALIZER;

void *lesInn(void *arg) {
    FILE *fil = fopen("inn.txt", "r");
    if (fil == NULL) {
        printf("Kunne ikke åpne filen\n");
        return NULL;
    }

    int beløp = 0;
    while (fscanf(fil, "%d", &beløp) != EOF) {
        pthread_mutex_lock(&balanse_mutex);
        balanse += beløp;
        pthread_mutex_unlock(&balanse_mutex);
    }
    fclose(fil);
    return NULL;
}

void* lesUt(void *arg) {
    FILE *fil = fopen("ut.txt", "r");
    if (fil == NULL) {
        printf("Kunne ikke åpne filen\n");
        return NULL;
    }

    int beløp = 0;
    while (fscanf(fil, "%d", &beløp) != EOF) {
        pthread_mutex_lock(&balanse_mutex);
        balanse -= beløp;
        pthread_mutex_unlock(&balanse_mutex);
    }
    fclose(fil);
    return NULL;
}

int main(int arch, char *argv[]) {
    pthread_t tråd_inn, tråd_ut;

    if(pthread_mutex_init(&balanse_mutex, NULL) != 0) {
        printf("Mutex init feilet\n");
        return 1;
    }

    if(pthread_create(&tråd_inn, NULL, &lesInn, NULL) != 0) {
        printf("Kunne ikke lage tråd for lesInn\n");
        return 2;
    }

    if(pthread_create(&tråd_ut, NULL, &lesUt, NULL) != 0) {
        printf("Kunne ikke lage tråd for lesUt\n");
        return 3;
    }

    if(pthread_join(tråd_inn, NULL) != 0) {
        printf("Kunne ikke hente tråd 1\n");
        return 2;
    }

    if(pthread_join(tråd_ut, NULL) != 0) {
        printf("Kunne ikke hente tråd 2\n");
        return 4;
    }

    printf("Balansen er: %d\n", balanse);

    pthread_mutex_destroy(&balanse_mutex);

    return 0;
}
