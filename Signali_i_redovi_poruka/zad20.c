/*Koriscenjem programskog jezika C napisati Linux program koji se moze prekinuti tako sto se u
roku od 1s kombinacijom tastera CTRL+C pretidne pet puta.Ukoliko istekne 1s,prethodno pritisnute
kombinacije tastera CTRL+C se ponistavaju. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int brojac=0;

void ctrlc()
{
    alarm(1);
    signal(SIGINT,ctrlc);
    brojac++;

    if(brojac==5)
    {
        printf("GASIM PROGRAM!\n");
        exit(0);
    }
}

void alarmproc()
{
    brojac = 0;
    printf("\n VREME JE ISTEKLO!\n");
}


int main()
{
    signal(SIGINT,ctrlc);
    signal(SIGALRM,alarmproc);

    for(;;)
        pause();
}