#include <stdio.h>
#include <unistd.h>

int main() {
int id;
int pipefd[2]; //[0]=read, [1]=write
int tabell[]={1,2,3,4,5,6,7,8,9,10,11,12};

// Oppretter pipe for begge prosessene og sjekker om det oppsto feil.
if (pipe(pipefd) == -1){
    printf("Kunne ikke opprette pipe\n");
    return 2;
}
//regner ut antall elementer og deler på 2. Gir antall elementer hver prosess skal summere.
int antall =(sizeof(tabell)/sizeof(int))/2;


// lager kopi av prosessen og sjekker om det oppsto feil
if((id = fork()) < 0)
{
    printf("Kunne ikke forke\n");
    return 1;
}

// sjekker om det er barneprosessen
if (id == 0){
    //printf("Child id=%d med prosessid %d\n",id,getpid());
    // setter verdier for loopen
    int start=0;
    int stop=start+antall;
    //lager variabler
    int sum=0;
    int f_p=0;
    // starter summering av halve tabellen
    for(int i=start;i<stop;i++)
    {
        sum = sum + tabell[i];
    }
    // Skriver ut ut summen  
    printf("Child: summen er %d\n",sum);
    // Leser fra pipen
    if(read(pipefd[0],&f_p,sizeof(f_p))<0){
        printf("Kunne ikke lese fra pipe\n");
        return 3;
    }
// Skriver ut
printf("summen av tallene er %d+%d=%d\n",sum,f_p,sum+f_p);
}

//foreldreprosessen
else{
    // setter variabler for loopen
    int start=antall;
    int stop=start+antall;
    int sum=0;
    // Summerer halve tabellen
    for(int i=start;i<stop;i++)
    {
        sum = sum + tabell[i];
    }
    printf("Parrent: summen er %d\n",sum);
    //Skriver til pipen og sjekker om feil
    if (write(pipefd[1], &sum, sizeof(sum))<0){
        printf("kunne ikke skrive til pipe\n");
    }
    //Venter på at barneprsessen skal avslutte
    wait(NULL);
    //printf("Parrent id=%d med prosessid %d\n",id,getpid());
}

//printf("id = %d\n",id);
//getchar();
return 0;
}