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
struct broj
{
    long mtype;
    int br[2];
};

struct recenica
{
    long mtype;
    char msg[100];
};

int main(int argc, char* argv[])
{
    int msqid;
    msqid = msgget(10000, 0666 | IPC_CREAT);
    struct broj broj;
    struct recenica red;
    broj.mtype = 1;
    red.mtype = 1;
    broj.br[0] = 0;
    broj.br[1] = 0;
    if(fork() != 0)
    {
        //roditelj
        FILE* f = fopen(argv[1], "r");
        while(!feof(f))
        {
            fgets(red.msg, 100, f);
            msgsnd(msqid, &red, strlen(red.msg)+1, 0);
            sleep(1);
            msgrcv(msqid, &broj, 50, 0,0);
            printf("Redni broj: %d, Broj velikih slova: %d\n", broj.br[0], broj.br[1]);
        }
        fclose(f);
        strcpy(red.msg, "kraj");
        msgsnd(msqid, &red, strlen(red.msg)+1, 0);
        printf("Kraj");
        wait(NULL);
        msgctl(msqid, IPC_RMID, 0);
    }
    else
    {
        int i;
        int br = 1;
        do
        {
            msgrcv(msqid, &red, 100,0,0);

            int s = 0;
            for(i = 0; i < strlen(red.msg); i++)
            {
                if(red.msg[i] >= 'A' && red.msg[i] <= 'Z')
                    s++;
            }
            broj.br[0] = br;
            broj.br[1] = s;
            //printf("%d,  %d\n", br, broj.br[1]);
            msgsnd(msqid, &broj, sizeof(broj.br),0 );
            br++;
        }while(strcmp(red.msg, "kraj") != 0);
    }
    return 0;
}
