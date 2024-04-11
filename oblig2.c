#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int balanse = 0;
//Mutex for å synkronisere tilgang til balanse
pthread_mutex_t balanse_mutex = PTHREAD_MUTEX_INITIALIZER;

void *lesInn(void *arg) {
    int* ptr_feil_kode = malloc(sizeof(int));
    *ptr_feil_kode = 0;
    
    FILE *fil = fopen("inn.txt", "r");
    if (fil == NULL) {
        printf("Kunne ikke åpne filen\n");
        *ptr_feil_kode = 1;
        return (void*) ptr_feil_kode;
    }

    int beløp = 0;
    while (fscanf(fil, "%d", &beløp) != EOF) {
        pthread_mutex_lock(&balanse_mutex);
        balanse += beløp;
        pthread_mutex_unlock(&balanse_mutex);
    }
    fclose(fil);
    return (void*) ptr_feil_kode;
}

void *lesUt(void *arg) {
    int* ptr_feil_kode = malloc(sizeof(int));
    *ptr_feil_kode = 0;

    FILE *fil = fopen("ut.txt", "r");
    if (fil == NULL) {
        printf("Kunne ikke åpne filen\n");
        *ptr_feil_kode = 1;
        return (void*) ptr_feil_kode;
    }

    int beløp = 0;
    while (fscanf(fil, "%d", &beløp) != EOF) {
        pthread_mutex_lock(&balanse_mutex);
        balanse -= beløp;
        pthread_mutex_unlock(&balanse_mutex);
    }
    fclose(fil);
    return (void*) ptr_feil_kode;
}

int main(int arch, char *argv[]) {
    pthread_t tråd_inn, tråd_ut;
    int* ret1 = NULL;
    int* ret2 = NULL;

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

    if(pthread_join(tråd_inn, (void**) &ret1) != 0) {
        printf("Kunne ikke hente tråd 1\n");
        return 4;
    }

    if(pthread_join(tråd_ut, (void**) &ret2) != 0) {
        printf("Kunne ikke hente tråd 2\n");
        return 5;
    }

    printf("Balansen er: %d\n", balanse);

    pthread_mutex_destroy(&balanse_mutex);
    free(ret1);
    free(ret2);

    return 0;
}