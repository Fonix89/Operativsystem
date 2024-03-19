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
    int debug=0;
    int tabell[5]={0,0,0,0,0};
    printf("argc = %d, argv = %s\n",argc,argv[0]);
    if (argc ==2){
        if (strcmp(argv[1], "1") == 0) debug=1;
    }
    int fd;
    int tall=0;
    if(mkfifo("file2",0755) == -1)
    {
        if (errno != EEXIST)
        {
            printf("Kunne ikke opprette filen\n");
            return 1;
        }
    }
    if (debug) printf("Opprettet fil\n");
    if (debug) printf("åpner filen for lesing\n");
    fd = open("file2", O_RDONLY);
    if (fd==-1)
    {
        printf("Kunne ikke åpne filen\n");
        return 2;
    }
    if (debug) printf("filen er åpen\n");
    if (debug) printf("Leser fra fil\n");
    if (read(fd, tabell, sizeof(tabell)) == -1)
    {
        printf("Kunne ikke lese til filen\n");
        return 3;
    }
    for(int i=0;i<5;i++) printf("Leste %d\n",tabell[i]);
    if (debug) printf("closer fil\n");
    close(fd);

    return 0;
}