/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji od korisnika očekuje da unese
username i password. Pošto korisnik unese username (bilo koja reč) ima 5 sekundi da unese i
password (takodje bilo koja reč). Ukoliko u tom roku korisnik unese i password program štampa
poruku “Uneli ste username i password.”. Ukoliko istekne 5 sekundi od unosa username-a program
štampa poruku “Vreme je isteklo!” i izlazi. Onemoguditi prekid izvršenja programa kombinacijom
taster Ctrl-C. */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void foo()
{
        printf("niste uneli password!Izlazim.");
        fflush(stdout);
        sleep(1);
        printf(".");
        fflush(stdout);
        sleep(1);
        printf(".");
        fflush(stdout);
        exit(0);
}

int main()
{
    signal(SIGALRM,foo);
    signal(SIGINT,SIG_IGN);
    char user[30],password[30];
    scanf("%s",user);
    alarm(5);
    scanf("%s",password);
    alarm(0);
    printf("uspesno ste uneli username i password");
    
}