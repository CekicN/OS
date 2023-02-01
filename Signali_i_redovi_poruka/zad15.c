/*
Korišćenjem programskog jezika C napisati Linux/UNIX program koji svom procesu detetu korišćenjem redova poruka prosleđuje stringove koje prihvata sa tastature, 
a proces dete primljene stringove upisuje u datoteku izlaz.txt. Komunikacija se prekida kada korisnik sa tastature unese reč KRAJ. 
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <wait.h>

#define MSG 10002
#define MAX 80

struct poruka
{
    long tip;
    char linija[MAX];
};

int main()
{
    int pid;
    int msgid;
    FILE *f;

    struct poruka buf;

    if((msgid=msgget(MSG, 0666 | IPC_CREAT)) < 0)
    {
        printf("Greska prilikom kreiranja reda poruka");
        return 0;
    }

    pid = fork();

    if(pid < 0)
    {
        printf("Greska prilikom forkovanja");
        return 0;
    }

    if(pid == 0) //dete
    {
        msgid=msgget(MSG, 0666);
        f=fopen("izlaz.txt","w");
        char pom[MAX];
        do
        {
            if(msgrcv(msgid,&buf,MAX,0,0) == -1)
            {
                printf("Greska prilikom prihvatanja");
                exit(0);
            }
            strcpy(pom,buf.linija);
            if(strcmp(pom,"KRAJ") != 0)
            {
                fprintf(f,"%s",buf.linija);
                // fprintf(f,"%s", "\n");
            }

        }while(strcmp(pom,"KRAJ") != 0);

    }else // roditelj
    {
        char unos[MAX]="";
        while(strcmp(unos,"KRAJ") != 0)
        {
            printf("Unesi string/KRAJ za kraj programa\n");
            gets(unos);
            buf.tip=1;
            strcpy(buf.linija,unos);
            if(msgsnd(msgid,&buf,strlen(buf.linija)+1,0) == -1)
            {
                printf("Greska prilikom slanja");
                exit(0);
            }
        }

        wait(NULL);
        msgctl(msgid,IPC_RMID,NULL);
        return 0;

    }
}