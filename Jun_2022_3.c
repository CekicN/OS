#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>

struct msg
{
    int type;
    int br;
};
int main()
{
    int msqid;
    struct msg m;
    int i;
    msqid = msgget(10000, 0666|IPC_CREAT);
    if(fork() == 0)
    {
        for(i = 0; i < 10; i++)
        {
            printf("Unesite broj:");
            scanf("%d", &m.br);

            msgsnd(msqid, &m, 4, 0);
        }
    }
    else
    {
        int s = 0;
        for(i = 0; i < 10; i++)
        {
            msgrcv(msqid, &m ,4,0, 0);

            int b = m.br;
            while(b > 0)
            {
                int c = b %10;
                b /= 10;
                s +=c;
            }
        }

        printf("rez je:%d", s);
        msgctl(msqid, IPC_RMID, NULL);
    }
    return 0;
}
