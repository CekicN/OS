/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji sa tastature učitava username i
zatim ga korišdenjem redova poruka šalje drugom procesu. Drugi proces za uneti username prvom
procesu vrada password (drugi proces za username pera vrada password peric, a za svaki drugi
username vrada NOT AVAILABLE). Zadatak rešiti korišdenjem JEDNOG reda poruka. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>

#define MSG_KEY 10100

struct msgbuf {
    long mtype;
    char poruka[30];
};

int main()
{
    int  msgID = msgget((key_t)MSG_KEY,0666 | IPC_CREAT);
    
    
    if(fork() != 0)
    {
        struct msgbuf buff;
        buff.mtype = 1;
        while(1)
        {
            scanf("%s",buff.poruka);
            

                
            msgsnd(msgID,&buff,sizeof(buff) - sizeof(long),0);
            if(strcmp(buff.poruka,"QUIT") == 0)
                    break;
            msgrcv(msgID,&buff,sizeof(buff) - sizeof(long),0,0);
            printf("password: %s\n",buff.poruka);
            
        }
        
        msgctl(msgID,IPC_RMID,0);
        
    }
    else
    {
        struct msgbuf buff;
        buff.mtype = 1;
        while(1)
        {
            msgrcv(msgID,&buff,sizeof(buff) - sizeof(long),0,0);
            
            if(strcmp(buff.poruka,"QUIT") == 0)
                break;
            
            if(strcmp(buff.poruka,"pera") == 0)
                strcpy(buff.poruka,"peric");
            else
                strcpy(buff.poruka,"NOT AVALABLE");
            msgsnd(msgID,&buff,sizeof(buff) - sizeof(long),0);
            
        }
    }
    
    
}