/* Koriscenjem programskog jezika C napisati Linux program koji od korisnika ocekuje da unese korisnicko ime. 
Ukoliko korisnik ne unese nista posle 30 sekundi, proces obavestava korisnika da je vreme isteklo i izlazi. */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

void catch_alarm(int sig_num)
{
    printf("Vreme je isteklo, izlazim. \n");
    exit(0);
}

char ime[40];

int main()
{
    signal(SIGALRM, catch_alarm);
    printf("Username: ");
    fflush(stdout);
    alarm(10);
    gets(ime);
    alarm(0);
    printf("Username: %s", ime);
    return 0;
}