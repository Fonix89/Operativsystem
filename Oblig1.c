#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pipe_child[2]; //[0] = read, [1] = write
    int pipe_parent[2]; //[0] = read, [1] = write
    int num;
    int guess;
    int feedback;
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
        close(pipe_child[1]);
        close(pipe_parent[0]); //Lukker leseenden av pipen i barneprosessen

        srand(time(NULL) ^ getpid());
        num = rand() % 1000 + 1;

        printf("Dette er tallet: %d\n", num);

        while(!found) {
            read(pipe_child[0], &guess, sizeof(guess));
            if (guess > num) {
                feedback = -1;
            } else if (guess < num) {
                feedback = 1;
            } else {
                feedback = 0;
                found = 1;
            }
            write(pipe_parent[1], &feedback, sizeof(feedback));
        }
        close(pipe_parent[1]); //Lukker skriveenden etter å ha sendt tallet
        close(pipe_child[0]);
        exit(0);
    } else {
        //Dette er foreldreprosessen
        close(pipe_child[0]); //Lukker skriveende av pipen i foreldreprosessen
        close(pipe_parent[1]); //Leser tallet 

        while(!found) {
            guess = low + (high - low) / 2;
            write(pipe_child[1], &guess, sizeof(guess));
            read(pipe_parent[0], &feedback, sizeof(feedback));
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
        close(pipe_child[1]);
        close(pipe_parent[0]);
        wait(NULL);
    } 
    return 0;
}