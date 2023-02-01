/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji se deli u dva procesa koji
komuniciraju korišdenjem reda poruka. Prvi proces periodično generiše N celih brojeva (N je u opsegu
od 1 do 10) i drugom procesu korišdenjem reda poruka šalje N i generisane brojeve. Drugi proces po
prijemu poruke štampa generisane celobrojen vrednosti na standardnom izlazu. Čitavu komunikaciju
treba ponoviti bar 5 puta. */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>

#define RED_PORUKA 10110

struct poruka {
    int N;
    int niz[10];
};

struct msgbuf{
    long mtype;
    struct poruka p;
};


int main()
{
            int msgID = msgget((key_t)RED_PORUKA,0666 | IPC_CREAT);
            
            if(fork() != 0)
            {
                int i,j;
                struct msgbuf buf;
               buf.mtype = 1; 
                for(i=0;i< 10;i++)
                {
                        buf.p.N = rand() % 10 + 1;
                        printf("Roditelj generisao N:%d\nRoditelj generisao niz:\n",buf.p.N);
                        for(j = 0;j < buf.p.N;j++)
                        {
                            buf.p.niz[j] = rand() % 30 + 10;
                            printf("%d ",buf.p.niz[j]);
                        }
                        printf("\n");
                        msgsnd(msgID,&buf,sizeof(buf) - sizeof(long),0);
                        sleep(3);
                }
                
                wait(NULL);
                msgctl(msgID,0,IPC_RMID);
            }
            else
            {
                    int i,j;
                    struct msgbuf buf;
                    for(i=0;i<10;i++)
                    {
                        msgrcv(msgID,&buf,sizeof(buf) - sizeof(long),1,0);
                        printf("Dete primilo %d\nClanovi niza su:\n",buf.p.N);
                        for(j = 0 ; j < buf.p.N;j++)
                            printf("%d ",buf.p.niz[j]);
                        printf("\n");
                        sleep(3);
                    }
                    
                
            }
}