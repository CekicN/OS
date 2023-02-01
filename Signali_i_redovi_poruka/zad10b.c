#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
struct msgbuf
{
    long mtype;
    char mtext[30];
};
int main()
{
    int msgqid;
    msgqid = msgget(10205, 0666 | IPC_CREAT);
    if(msgqid < 0)
    {
        printf("Doslo je do greske prilikom kreiranja reda poruka!\n");
        return 0;
    }
    struct msgbuf buf;
    msgrcv(msgqid, &buf, 30, 1, 0);
    while(strcmp(buf.mtext, "KRAJ") != 0)
    {
        printf("Procitano: %s\n", buf.mtext);
        msgrcv(msgqid, &buf, 30, 1, 0);
    }
}