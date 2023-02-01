/*Koriscenjem programskog jezika C napraviti Linux program koji moze da se prekine samo ako se 5 puta uzastopno pritisne kombinacija CTRL-C.
Ukoliko korisnik pritisne kombinaciju CTRL-Z, onda se na ekranu ispisuje poruka koliko puta do sada je pritisnuto CTRL+C. */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int pritisnutoCTRLC=0;

void catch_int(int sig_num)
{
    signal(SIGINT,catch_int);
    pritisnutoCTRLC++;
    if (pritisnutoCTRLC>=5)
        exit(0);
    else
        pritisnutoCTRLC=0;

}

void catch_suspend(int sig_num)
{
    signal(SIGTSTP, catch_suspend);
    printf("Ukupno pritisnuto CTRL+C = %d/n", pritisnutoCTRLC);
    fflush(stdout);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, catch_int);
    signal(SIGTSTP, catch_suspend);

    for(;;);
    pause();
}