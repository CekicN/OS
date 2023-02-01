/*
Korišćenjem programskog jezika C napisati UNIX/Linux program koji može da se prekine samo ako se kombinacija tastera Ctrl-C pritisne pet puta za redom. 
Ukoliko korisnik pritisne kombinaciju tastera Ctrl-Z dva puta zaredom program na ekranu ispisuje koliko puta do sada je pritisnuto Ctrl-C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int brojac=0;
int b=0;

void ctrlc()
{
    signal(SIGINT,ctrlc);
    brojac++;
    b=0;

    if(brojac==5)
    {
        char k;
        printf("\nDa li zelite da prekinete program? y/n\n");
        scanf("%c",&k);
        if(k=='y')
            exit(0);
        else brojac=0;
    }
}

void ctrlz()
{
    b++;
    signal(SIGTSTP,ctrlz);
    if(b%2==0)
        printf("\nBrojac = %d\n",brojac);
}


int main()
{
    signal(SIGINT,ctrlc);
    signal(SIGTSTP,ctrlz);

    for(;;)
        pause();
}


