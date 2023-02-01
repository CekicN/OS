/*
Korišćenjem programskog jezika C napisati UNIX/Linux program koji svom procesu detetu korišćenjem redova poruka identifikovan brojem 12001 prosleđuje 
celobrojne vrednosti različite od nule koje čita sa tastature. Poruke su tipa 22. 
Proces dobijene vrednosti sumira dok ne dobije vrednost 0. Tada na ekranu štampa sumu i izlazi. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG 12001
#define TIP 22

struct poruka
{
    long tip;
    int broj;
};

int main()
{
    int pid,msgid;

    struct poruka buf;

    if(msgid=(msgget(MSG, 0666 | IPC_CREAT)) < 0)
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

    if(pid == 0)
    {
        int s=0;
        int w;
        msgid=(msgget(MSG, 0666));
        do
        {

            if(msgrcv(msgid,&buf,sizeof(int),22,0) == -1)
            {
                printf("Greska prilikom prihvatanja\n");
                exit(0);
            }

            s+=buf.broj;
            w=buf.broj;

        }while(w != 0);

        printf("Suma je: %d\n", s);
        exit(0);
    }
    else
    {
        int k=1;
        int b=1;
        msgid=(msgget(MSG, 0666));
        while(b!=0)
        {
            printf("Unesi broj:\n");
            scanf("%d",&b);
            buf.broj=b;
            buf.tip=22;

            if(msgsnd(msgid,&buf,sizeof(int),0) == -1)
            {
                 printf("Greska prilikom slanja\n");
                 exit(0);
            }

        }

        wait();
        msgctl(msgid,IPC_RMID,NULL);
        return 0;
    }

}


