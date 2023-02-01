/*Koriscenjem programskog jezika C napisati dva Linux procesa koja komuniciraju koriscenjem redova poruka. 
Prvi proces cita stringove sa tastature i upisuje ih u red poruka sve dok se na tastaturi ne unese rec KRAJ. 
Drugi proces cita stringove iz reda poruka i prikazuje ih na standardnom izlazu. 
U sistemu postoji i treci proces koji kreira red poruka i pokrece ova dva procesa, a zatim ceka da se oni zavrse,
i tek nakon toga brise red poruka i zavrsava svoje izvrsenje. */

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
        exit(-1);
    }
    struct msgbuf buf;
    if(fork() != 0)
    {
        execl("zad10a", "zad10a", NULL);
    }
    else
    {
        execl("zad10b", "zad10b", NULL);
    }
}