#include <stdio.h>
#include <unistd.h>

int main() {
    int pid = fork(); // Oppretter en ny prosess
    if (pid < 0) {
    // Feil ved forking
        printf("Fork feilet\n");
        return 1;
    } else if (pid == 0) {
        // Dette er barneprosessen
        printf("Dette er barneprosessen (PID: %d)\n", getpid());
    } else {
        // Dette er foreldreprosessen
        printf("Dette er foreldreprosessen (Barneprosess PID: %d)\n", pid);
    }
    return 0;
}