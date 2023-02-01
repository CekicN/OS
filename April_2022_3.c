#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<sys/wait.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<sys/wait.h>
struct msg
{
    long mtype;
    int br;
};
int main()
{
    srand(time(0));
    int i, msqid, N;
    struct msg m;
    m.mtype = 1;
    msqid = msgget(10000, 0666 | IPC_CREAT);
    if(fork() == 0)
    {
        N = rand() % 10 + 1;

        for(i = 0; i < N; i++)
        {
            m.br = rand() % 100+1;
            msgsnd(msqid, &m, 4, 0);
        }

        m.br = -1;
        msgsnd(msqid, &m, 4, 0);
    }
    else
    {
        int p = 0,n = 0;
        do
        {
            msgrcv(msqid, &m, 4, 0, 0);
            if(m.br % 2 == 0)
                p++;
            else
                n++;
            printf("Primio sam: %d\n", m.br);
        }while(m.br != -1);

        printf("Broj parnih je: %d, a neparnih: %d\n", p, n);
        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);
    }
    return 0;
}
