/* Koriscenjem programskog jezika C napisati Linux program koji ucitava podatke iz tekstualne datoteke cela red po red, a zatim koriscenjem reda poruka
sve parne redove salje procesu koji konvertuje sva slova u velika i upisuje u datoteku pola1, a neparne redove procesu koji konvertuje sva slova
u mala i upisuje u datoteku pola 2. */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>

#define RED_PORUKA 10001
#define MAX_PORUKA 10

struct poruka
{
    long tip;
    char tekst[MAX_PORUKA];
};

int main()
{
    int pid;
    int redid;
    struct poruka bafer;
    FILE* f;
    int poruka;
    char tmp[MAX_PORUKA];

    redid=msgget(RED_PORUKA, 0666 | IPC_CREAT);

    pid=fork();
    if (pid==0)
    {
        f=fopen("pola1.txt", "a");
        if (f==NULL)
            printf("Doslo je do greske prilikom otvaranja fajla");
        while (strcmp(bafer.tekst, "END")!=0)
        {
            if (msgrcv(redid, &bafer, MAX_PORUKA, 1, 0)==-1)
                printf("Doslo je do greske prilikom citanja poruke");
            
            if (strcmp(bafer.tekst, "END")!=0)
            {
                fprintf(f, "%s", bafer.tekst);
                fflush(f);
            }
        }
        fclose(f);

    }
    else
    {
        int pid2=fork();
        if (pid2==0)
        {
            f=fopen("pola2.txt", "a");
        if (f==NULL)
            printf("Doslo je do greske prilikom otvaranja fajla");
        while (strcmp(bafer.tekst, "END")!=0)
        {
            if (msgrcv(redid, &bafer, MAX_PORUKA, 2, 0)==-1)
                printf("Doslo je do greske prilikom citanja poruke");
            
            if (strcmp(bafer.tekst, "END")!=0)
            {
                fprintf(f, "%s", bafer.tekst);
                fflush(f);
            }
        }
        fclose(f);
        }
        else
        {
            f=fopen("cela.txt", "r");
            if (f==NULL)
            {
                printf("Doslo je do greske prilikom otvaranja fajla");
                exit(1);
            }
            poruka=0;
            fgets(tmp, MAX_PORUKA, f);
            do
            {
                strcpy(bafer.tekst, tmp);
                if (poruka%2==0)
                    bafer.tip=1;
                else
                    bafer.tip=2;

                if (msgsnd(redid, &bafer, MAX_PORUKA, 0)==-1)
                    printf("Doslo je do greske prilikom slanja poruke");
                poruka++;
                fgets(tmp, MAX_PORUKA, f);
            } 
            while (!feof(f));

            fclose(f);
            strcpy(bafer.tekst, "END");
            bafer.tip=1;
            msgsnd(redid, &bafer, MAX_PORUKA, 0);
            strcpy(bafer.tekst, "END");
            bafer.tip=2;
            msgsnd(redid, &bafer, MAX_PORUKA, 0);
            wait(NULL);
            wait(NULL);
            msgctl(redid, IPC_RMID, NULL);
            
        }
    }
}