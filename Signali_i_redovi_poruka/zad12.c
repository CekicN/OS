/*Koriscenjem programskog jezika C napisati Linux program koji kreira dodatni proces dete. 
Ova dva procesa komuniciraju dvosmerno koriscenjem jednog reda poruka. Prvi proces sa tastature cita cele brojeve
u vidu stringa i salje ih drugom procesu koriscenjem reda poruka. Drugi proces tako dobijeni broj mnozi sa 2, 
i rezultat vraca prvom procesu. Prvi proces tako dobijeni broj stampa na ekranu. Ovaj postupak se ponavlja 10 puta. */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define RED_PORUKA 10001
int n;

struct poruka
{
    long tip;
    char tekst[100];
};

int main()
{
    n = 0;
    int redid = msgget(RED_PORUKA, IPC_CREAT | 0666);
    if(fork() == 0)
    {
        //dete
        struct poruka buff;
        while(n < 20)
        {
            
            msgrcv(redid, &buff, sizeof(struct poruka), 0, 0);
            if(buff.tip == 1)
            {
                printf("Dete primilo :");
                printf("%s\n",buff.tekst);
                int broj = atoi(buff.tekst);
                broj = broj * 2;
                sprintf(buff.tekst, "%d", broj);
                buff.tip = 2;
                printf("Dete poslalo: ");
                printf("%s\n", buff.tekst);
                msgsnd(redid, &buff, sizeof(struct poruka), 0);
            }
        }
        
    }
    else
    {
        //roditelj
        struct poruka buff;
        while(n < 20)
        {
            gets(buff.tekst);
            printf("Roditelj sa ulaza: ");
            puts(buff.tekst);
            buff.tip = 1;
            msgsnd(redid, &buff, sizeof(struct poruka), 0);
            msgrcv(redid, &buff, sizeof(struct poruka), 0, 0);
            
            if(buff.tip == 2)
            {
                printf("Roditelj primio nazad: ");
                puts(buff.tekst);
            }
                
            n++;
        }
        wait(NULL);
        msgctl(redid, IPC_RMID, 0);
    }
    return 0;
}