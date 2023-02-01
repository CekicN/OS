/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji je nemogude prekinuti ili
suspendovati korišdenjem kombinacije tastera Ctrl-C i Ctrl-Z. Proces je mogude prekinuti samo ukoliko
se ove kombinacija tastera pritisnu u određenoj sekvenci (Npr. Ctrl-C, Ctrl-C, Ctrl-Z, Ctrl-C). Ukoliko
korisnik pritisne pogrešnu kombinaciju tastera, sekvenca se resetuje i korisnik se o tome obaveštava. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int napredak = 0;
void sekvencaC();
void sekvencaZ();
int main()
{
  signal(SIGINT,sekvencaC);
  signal(SIGTSTP,sekvencaZ);

    while(1)
        pause();
}


void sekvencaC()
{
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    
    if(napredak >= 0 && napredak <= 3)
        napredak ++;
    else if(napredak == 5)
    {
        printf("Napredak 100%\n\n GASENJE PROGRAMA!\n");
        exit(0);
    }
    else
        napredak = 0;
    
    printf("Vas napredak u gasenju programa %d/6\n",napredak);
    fflush(stdout);
    
      signal(SIGINT,sekvencaC);
    signal(SIGTSTP,sekvencaZ);
}


void sekvencaZ()
{
        signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    if(napredak == 4)
        napredak ++;
    else
        napredak = 0;
    
     printf("Vas napredak u gasenju programa %d/6\n",napredak);
    fflush(stdout);

          signal(SIGINT,sekvencaC);
    signal(SIGTSTP,sekvencaZ);
}