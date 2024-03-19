#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int debug = 0;
    int fd;
    int tall = 97;


    if (mkfifo("file1", 0777) == -1) {
        if (errno != EEXIST) {
            printf("Kunne ikke opprette filen\n");
            return 1;
        }
    }

    printf("Opprettet fil\n");
    printf("åpner filen for skriving\n");
    fd = open("file1", O_WRONLY);
    if (fd == -1) {
        printf("Kunne ikke åpne file\n");
        return 2;
    }

    printf("Filen er åpen\n");
    printf("Skriver til fil\n");
    if (write(fd, &tall, sizeof(tall)) == -1) {
        printf("Kunne ikke skrive til filen");
        return 3;
    }

    printf("Stenger fil\n");
    close(fd);
    return 0;
}