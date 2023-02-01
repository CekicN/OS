/* Koriscenjem programskog jezika C napisati Linux program koji simulira problem proizvodjac/potrosac koriscenjem redova poruka. 
Glavni program se deli u dva procesa. Proizvodjac kreira N slucajnih pozitivnih celih brojeva i salje ih drugom procesu.
N se odredjuje tokom izvrsenja, takodje kao slucajan broj. Po zavrsetku slanja, prvi proces salje -1 za kraj. 
Drugi proces preuzima brojeve i stampa ih na standardnom izlazu. */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>

#define RED_PORUKA 10001
#define MAX_PORUKA 10

struct poruka
{
    long tip;
    char tekst[MAX_PORUKA];
};

int main()
{
    int pid;
    int redid;
    int broj=0;
    struct poruka bafer;

    pid=fork();
    redid=msgget(RED_PORUKA, 0666 | IPC_CREAT);
    if (redid<0)
    {
        printf("Doslo je do greske prilikom kreiranja reda");
        exit(1);
    }

    if (pid<0)
        printf("Doslo je do greske prilikom kreiranja procesa");
    
    if (pid==0)
    {
        int n=rand()%20;
        for (int i=0; i<n+1; i++)
        {
            if (i==n)
            {
                sprintf(bafer.tekst, "%d", -1);
            }
            else
            {
                sprintf(bafer.tekst, "%d", rand()%100);
                printf("Poslato: ");
                printf("%s\n", bafer.tekst);
            }

            bafer.tip=1;
            if (msgsnd(redid, &bafer, sizeof(bafer.tekst), 0))
            {
                printf("Doslo je do greske kod slanja poruke");
                exit(1);
            }
        }
        
    }

    else
    {
        do
        {
            if(msgrcv(redid, &bafer, MAX_PORUKA,0,0)==-1)
            {
                printf("Doslo je do greske");
            }
            broj=atoi(bafer.tekst);
            printf("%d\n", broj);
        } 
        while (broj>-1);

        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
        
    }
}