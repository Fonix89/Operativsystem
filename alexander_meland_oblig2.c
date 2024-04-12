#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

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
        return (void *) ptr_feil_kode;
    }

    char beløp_inn[100];
    while (fscanf(fil, "%s", &beløp_inn) != EOF) {
        int i = 0;
        while(beløp_inn[i] != '\0') {
            if (isdigit(beløp_inn[i]) == 0) {
                printf("Ugyldig input\n");
                fclose(fil);
                *ptr_feil_kode = 2;
                return (void *) ptr_feil_kode;
            }
            i++;
        }
        pthread_mutex_lock(&balanse_mutex);
        balanse += atoi(beløp_inn);
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
        return (void *) ptr_feil_kode;
    }

    char beløp_ut[100];
    while (fscanf(fil, "%s", &beløp_ut) != EOF) {
        int i = 0;
        while(beløp_ut[i] != '\0') {
            if (isdigit(beløp_ut[i]) == 0) {
                printf("Ugyldig input\n");
                fclose(fil);
                *ptr_feil_kode = 2;
                return (void *) ptr_feil_kode;
            }
            i++;
        }
        pthread_mutex_lock(&balanse_mutex);
        balanse -= atoi(beløp_ut);
        pthread_mutex_unlock(&balanse_mutex);
        }
    fclose(fil);
    return (void*) ptr_feil_kode;
}

int main(int arch, char *argv[]) {
    pthread_t tråd_inn, tråd_ut;
    int* ret1 = NULL;
    int* ret2 = NULL;

    if(pthread_create(&tråd_inn, NULL, &lesInn, NULL) != 0) {
        printf("Kunne ikke lage tråd for lesInn\n");
        return 3;
    }

    if(pthread_create(&tråd_ut, NULL, &lesUt, NULL) != 0) {
        printf("Kunne ikke lage tråd for lesUt\n");
        return 4;
    }

    if(pthread_join(tråd_inn, (void**) &ret1) != 0) {
        printf("Kunne ikke hente tråd 1\n");
        return 5;
    }

    if(pthread_join(tråd_ut, (void**) &ret2) != 0) {
        printf("Kunne ikke hente tråd 2\n");
        return 6;
    }

    printf("Balansen er: %d\n", balanse);

    pthread_mutex_destroy(&balanse_mutex);
    free(ret1);
    free(ret2);

    return 0;
}
