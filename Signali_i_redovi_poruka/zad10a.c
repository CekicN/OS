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
    buf.mtype = 1;
    char red[30];
    strcpy(red, "");
    while(strcmp(red, "KRAJ") != 0)
    {
        gets(red);
        strcpy(buf.mtext, red);
        msgsnd(msgqid, &buf, strlen(buf.mtext) + 1, 0);
    }
}