#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>       
#include <sys/stat.h>

int main(int argc, char* argv[0]) {
    int tabell[5]={1,2,3,4,5};
    int debug=0;
    printf("argc = %d, argv = %s\n",argc,argv[0]);
    if (argc ==2){
        if (strcmp(argv[1], "1") == 0) debug=1;
    }
    int fd;
    int tall=97;
    if(mkfifo("file2",0755) == -1)
    {
        if (errno != EEXIST)
        {
            printf("Kunne ikke opprette filen\n");
            return 1;
        }
    }
    if (debug) printf("Opprettet fil\n");
    if (debug) printf("åpner filen for skriving\n");
    fd = open("file2", O_WRONLY);
    if (fd==-1)
    {
        printf("Kunne ikke åpne filen\n");
        return 2;
    }
    if (debug) printf("filen er åpen\n");
    if (debug) printf("Skriver til fil\n");
    if (write(fd, tabell, sizeof(int)*5) == -1)
    {
        printf("Kunne ikke skrive til filen\n");
        return 3;
    }
    if (debug) printf("closer fil\n");
    close(fd);

    return 0;
}