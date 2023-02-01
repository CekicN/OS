/*
Korišćenjem programskog jezika C napisati Linux program koji treba da detektuje i obradi signale tipa SIGALRM i SIGINT. 
Kada proces detektuje signal tipa SIGALRM on korišćenjem reda poruka (koji je definisan ključem 10500) šalje poruku (tip 20) koja sadrži informaciju o tekućem vremenu. 
U slučaju pojave SIGINT signala (pritisnuta kombinacija tastera Ctrl+C) proces suspenduje svoje izvršavanje na 5 sekundi a nakon toga briše red poruka 
(koji je koristio za slanje informacija o vremenu) i prekida svoje izvršavanje. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define MSG 10500
#define L 100

struct poruka
{
    long tip;
    char vreme[L];
};

int msgid;
struct poruka buf;

void alr()
{
    msgid=msgget(MSG,0666);
    time_t t;
    //struct tm *timeinfo;

    time(&t);
    strcpy(buf.vreme,ctime(&t));
    buf.tip=20;

    if(msgsnd(msgid,&buf,strlen(buf.vreme),0) == -1)
    {
        printf("Greska prilokom slanja\n");
        exit(0);
    }

}

void ctrlc()
{
    sleep(5);
    printf("\nUso u ctrl c");
    msgctl(msgid,IPC_RMID,NULL);
    exit(0);
}
int main()
{
    signal(SIGALRM,alr);
    signal(SIGINT,ctrlc);
    msgid=msgget(MSG,0666 | IPC_CREAT);
    alarm(5);
    pause();
    if(msgrcv(msgid,&buf,L,20,0) == -1)
    {
        printf("Greska prilokom citanja\n");
        exit(0);
    }

    printf("Trenutno vreme je: %s\n",buf.vreme);

}