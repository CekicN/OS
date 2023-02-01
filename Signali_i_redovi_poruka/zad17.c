/*
Korišćenjem programskog jezika C napisati UNIX/Linux program koji od korisnika očekuje da sa tastature unese korisničko ime i lozinku. 
Ukoliko korisnik nakon unosa korisnickog imena ne unese lozinku u roku od 60 sekundi proces
obaveštava korisnika da je vreme za unos lozinke isteklo i izlazi.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>


void kraj()
{
    printf("Vreme za unos je isteklo!\n");
    exit(0);
}

int main()
{
    signal(SIGALRM,kraj);
    char ime[30];
    char lozinka[30];
    alarm(10);
    printf("Unesi ime:\n");
    fflush(stdout);
    gets(ime);

    printf("Unesi lozinku:\n");
    fflush(stdout);
    gets(lozinka);

    alarm(0);

    printf("Uneli ste ime: %s i lozinku: %s\n",ime,lozinka);


    return 0;
}