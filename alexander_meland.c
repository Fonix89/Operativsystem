#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main(int argc, char* argv[]) {

    int pipe_child[2]; //[0] = read, [1] = write
    int pipe_parent[2]; //[0] = read, [1] = write
    int num = 0;
    int guess = 0;
    int feedback = 0;
    int number_of_guesses = 0;
    int low = 0, high = 1000, found = 0;

    if(pipe(pipe_child) == -1 || pipe(pipe_parent) == -1) {
        printf("Kunne ikke opprette pipe\n");
        return 1;
    }

    int prosess_id = fork();

    if (prosess_id < 0) {
        //Feil ved forking
        printf("Fork feilet\n");
        return 2;
    }
    
    if (prosess_id == 0) {
        //Dette er barneprosessen

        close(pipe_child[WRITE]);
        close(pipe_parent[READ]); //Lukker leseenden av pipen i barneprosessen

        srand(time(NULL) ^ getpid());
        num = (rand() % 999) + 1;

        while(!found) {
            if (read(pipe_child[0], &guess, sizeof(guess)) == -1) {
                printf("Kunne ikke lese fra pipe\n");
                return 3;
            }

            if (guess > num) {
                feedback = -1;
            } else if (guess < num) {
                feedback = 1;
            } else {
                feedback = 0;
                found = 1;
            }

            if (write(pipe_parent[1], &feedback, sizeof(feedback)) == -1) {
                printf("Kunne ikke skrive til pipe\n");
                return 4;
            }
        }
        close(pipe_parent[WRITE]); //Lukker skriveenden etter å ha sendt tallet
        close(pipe_child[READ]);
        
    } else {
        //Dette er foreldreprosessen
        close(pipe_child[READ]); //Lukker skriveende av pipen i foreldreprosessen
        close(pipe_parent[WRITE]); //Leser tallet 

        while(!found) {
            guess = low + (high - low) / 2;
            if(write(pipe_child[WRITE], &guess, sizeof(guess)) == -1) {
                printf("Kunne ikke skrive til pipe\n");
                return 5;
            }

            if(read(pipe_parent[READ], &feedback, sizeof(feedback)) == -1) {
                printf("Kunne ikke lese fra pipe\n");
                return 6;
            }

            number_of_guesses++;

            if (feedback == 1) {
                low = guess + 1;
            } else if (feedback == -1) {
                high = guess - 1;
            } else {
                found = 1;
                printf("Korrekt tall: %d funnet etter %d forsøk\n", guess, number_of_guesses);
            }
        }
        close(pipe_child[WRITE]);
        close(pipe_parent[READ]);
        wait(NULL);
    } 
    return 0;
}