/*
Korišćenjem programskog jezika C napisati UNIX/Linux program koji simulira problem proizvođač/potrošač korišćenjem redova poruka (message-queues). 
Glavni program se deli u dva procesa. Prvi proces (proizvođač) kreira N slučajnih pozitivnih celih brojeva i šalje ih drugom procesu. 
N se određuje tokom izvršenja, takođe kao slučajan pozitivan ceo broj. Po završetku slanja, prvi proces šalje -1 kao kod za kraj. 
Drugi proces (potrošač) preuzima poslate brojeve iz poruka i štampa ih na standardnom izlazu. 
*/

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

#define MSG 21001
#define MAX 10

struct poruka
{
    long mtype;
    int broj;
};

int main()
{
    srand(time(NULL));
    int pid;
    int msgid;
    int N=rand()%10;
    struct poruka buf;

    msgid = msgget(MSG, 0666 | IPC_CREAT);
    if(msgid < 0)
    {
        printf("Greska prilikom kreiranja reda\n");
        exit(0);
    }

    pid=fork();

    if(pid < 0)
    {
        printf("Greska prilikom forkovanja\n");
        exit(0);
    }

    if(pid == 0) // dete
    {
        for(int i=0;i<N+1;i++)
        {
            if(i!=N)
            {
                buf.broj=rand()%100;
            }
            else buf.broj=-1;

            buf.mtype=1;

            if(msgsnd(msgid,&buf,sizeof(int),0) == -1)
            {
                printf("Greska prilikom slanja\n");
                exit(0);
            }
        }

    }
    else // roditelj
    {
        int b;
        for(int i=0;i<N+1;i++)
        {
            if(msgrcv(msgid,&buf,sizeof(int),0,0) == -1)
            {
                printf("Greska prilikom prihvatanja\n");
                return 0;
            }
            printf("Primljeni broj je: %d\n",buf.broj);
        }

        wait(NULL);
        msgctl(MSG,IPC_RMID,0);
        return 0;
    }
}