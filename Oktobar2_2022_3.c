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
#include<ctype.h>
struct poruka
{
    int type;
    char msg[100];
};
int main()
{
    int msqid;
    struct poruka msg;
    msqid = msgget(10000, 0666 | IPC_CREAT);
    msg.type = 3;
    strcpy(msg.msg, "");
    if(fork() == 0)
    {
        //1
        int br;
        do
        {
            printf("Unesite broj(1 ili 2): ");
            scanf("%d", &br);
            getc(stdin);
            gets(msg.msg);

            if(br == 1)
            {
                msg.type = 1;
            }
            else if(br == 2)
            {
                msg.type = 2;
            }
            msgsnd(msqid, &msg, strlen(msg.msg)+1, 0);
        }while(br == 1 || br == 2);

        msg.type = 1;
        strcpy(msg.msg, "kraj");
        msgsnd(msqid, &msg, strlen(msg.msg)+1, 0);
        msg.type = 2;
        strcpy(msg.msg, "kraj");
        msgsnd(msqid, &msg, strlen(msg.msg)+1, 0);
    }
    if(fork() == 0)
    {
        //2
        do
        {
            msgrcv(msqid, &msg, 100,1,0);

            printf("Proces 2:Poruka je: %s, Velicina poruke: %ld\n", msg.msg, strlen(msg.msg));
        }while(strcmp(msg.msg, "kraj") == 0);
    }
    else
    {
        //3
        do
        {
            msgrcv(msqid, &msg, 100,2,0);
            printf("%s", msg.msg);
            int i = 0;
            while(msg.msg[i] != '\0')msg.msg[i] = toupper(msg.msg[i++]);

            printf("Proces3: Poruka je: %s\n", msg.msg);
        }while(strcmp(msg.msg, "kraj") == 0);
        wait(NULL);
        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);
    }
}
