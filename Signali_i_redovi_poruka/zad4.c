/*Koriscenjem programskog jezika C napisati Linux program koji svom procesu detetu, koriscenjem redova poruka, prosledjuje ulaz koji prima preko tastature, 
a proces dete dobijene poruke stampa na ekranu. Unosom teksta "quit" se prekida rad programa. */


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>

#define MSGLEN 50
#define KEY 10104

struct mymsgbuf
{
    long mtype;
    char mtext[MSGLEN];
};


main()
{
    int msqid;
    struct mymsgbuf buf;

    buf.mtype=1;
    strcpy(buf.mtext, "");

     msqid=msgget(KEY, 0666 | IPC_CREAT);

     if (fork()==0)
     {
         printf("Dete");
         while(strcmp(buf.mtext, "quit")!=0)
         {
             msgrcv(msqid, &buf, 50, 0, 0);
             printf("Proces dete: %s\n", buf.mtext);
             //fflush(stdout);
         }
     }

     else
     {
         scanf("%s", buf.mtext);
         while (strcmp(buf.mtext, "quit")!=0)
         {
             scanf("%s", buf.mtext);
            msgsnd(msqid, &buf, strlen(buf.mtext)+1, 0);
            printf("Proces roditelj: %s\n", buf.mtext);
            //fflush(stdout);
         }
        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);
     }
     

}