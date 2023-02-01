/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji svom procesu detetu 
korišćenjem redova poruka identifikovan brojem 12010 prosleđuje karaktere unete sa tastature. 
Poruke su tipa 4. Proces dobijene vrednosti upisuje u datoteku karakteri.txt. 
Komunikacija se prekida kada korisnik unese bilo koji broj.*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#define KOD 12010
struct mybuf{
    long type;
    char mtext[1];
};

void main()
{
   int msqid;
   struct mybuf buf;

   buf.type =4;
   buf.mtext[0] = ' ';

   msqid = msgget(KOD, 0666 | IPC_CREAT);

   if(fork()==0)
   {
       FILE *fajl = fopen("karakter.txt","w");
       while(buf.mtext[0]<'0'||buf.mtext[0]>'9')
       {
           msgrcv(msqid, &buf, 1,4,0);
            if(buf.mtext[0]<'0'||buf.mtext[0]>'9')
            fprintf(fajl,"%c",buf.mtext[0]);
       }
       fclose(fajl);
   }
   else
   {
       while(buf.mtext[0]<'0'||buf.mtext[0]>'9')
       {
           printf("Unesite karakter: ");
           scanf("%c",&buf.mtext[0]);
            msgsnd(msqid,&buf,1,0);
       }
       wait(NULL);
       msgctl(msqid,IPC_RMID,NULL);
   }
}