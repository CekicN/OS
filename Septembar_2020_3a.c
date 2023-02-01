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

struct poruka
{
    long type;
    char msg[100];
};
int main(int argc, char* argv[])
{
    int msqid = atoi(argv[1]);
    struct poruka poruka;
    poruka.type = 1;
    if(fork() == 0)
    {
        do
        {
            gets(poruka.msg);
            msgsnd(msqid, &poruka, strlen(poruka.msg)+1, 0);

        }while(strcmp(poruka.msg, "kraj") != 0);
    }
    else
    {
        do
        {
            msgrcv(msqid, &poruka, 100,0, 0);
            printf("%s\n", poruka.msg);

        }while(strcmp(poruka.msg, "kraj") != 0);
        wait(NULL);
    }
}
