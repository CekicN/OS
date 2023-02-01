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


int main()
{

    int msqid = msgget(10000, 0666 | IPC_CREAT);
    char buf[50];
    sprintf(buf, "%d", msqid);
    if(fork() == 0)
    {
        execl("Septembar_2020_3a", "Septembar_2020_3a",buf, NULL);
    }
    else
    {
        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);
    }
}
