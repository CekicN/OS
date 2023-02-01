/*
Korišćenjem programskog jezika C napisati UNIX/Linux program koji kreira dva dodatna procesa. 
Prvi proces dete čita liniju po liniju iz datoteke ulaz1.txt. 
Drugi proces dete čita liniju po liniju iz datoteke ulaz2.txt. 
Oba procesa deteta prosleđuju pročitane linije prosleđuju roditeljskom procesu koristeći deljenu memoriju. 
Roditeljski proces primljene linije teksta od oba procesa deteta upisuje u tekstualnu datoteku izlaz.txt naizmenično.
(Uvek odredi kod za kraj)
*/



#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LINE_LENGTH 80
#define PROCA_TURN 10001
#define PROCB_TURN 10002
#define WRITE_TURN 10003
#define SHARED_MEM 10005

union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
    struct seminfo *__buf;
    void *__pad;
};

int main()
{
    int procaid, procbid, writeid, memid;
    char *shmem;
    union semun opts;
    int turn, procAdone, procBdone;
    FILE *file;

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};

    procaid = semget(PROCA_TURN, 1, 0666 | IPC_CREAT);
    procbid = semget(PROCB_TURN, 1, 0666 | IPC_CREAT);
    writeid = semget(WRITE_TURN, 1, 0666 | IPC_CREAT);

    memid = shmget(SHARED_MEM, LINE_LENGTH * sizeof(char), 0666 | IPC_CREAT);

    opts.val = 1;
    semctl(procaid, 0, SETVAL, opts);

    opts.val = 0;
    semctl(procbid, 0, SETVAL, opts);

    opts.val = 0;
    semctl(writeid, 0, SETVAL, opts);

    if (fork() == 0) //1. dete
    {
        int done = 1;

        procaid = semget(PROCA_TURN, 1, 0666);
        procbid = semget(PROCB_TURN, 1, 0666);
        writeid = semget(WRITE_TURN, 1, 0666);

        shmem = (char *)shmat(memid, NULL, 0);

        file = fopen("ulaz1.txt", "r");

        do
        {
            semop(procaid, &lock, 1);               //blokira procaid
            if (!(fgets(shmem, LINE_LENGTH, file))) //ukoliko procita EOF onda je kraj
            {
                done = 0;
                shmem[0] = 0;
            }
            semop(writeid, &unlock, 1); //aktivira roditelja da tu liniju procita
        } while (done);

        fclose(file);
        shmdt(shmem);
        exit(0);
    }
    if (!fork())
    {
        int done = 1;
        procaid = semget(PROCA_TURN, 1, 0666);
        procbid = semget(PROCB_TURN, 1, 0666);
        writeid = semget(WRITE_TURN, 1, 0666);

        shmem = (char *)shmat(memid, NULL, 0);

        file = fopen("ulaz2.txt", "r");

        do
        {
            semop(procbid, &lock, 1);
            if (!(fgets(shmem, LINE_LENGTH, file)))
            {
                done = 0;
                shmem[0] = 0;
            }
            semop(writeid, &unlock, 1);
        } while (done);

        fclose(file);
        shmdt(shmem);
        exit(0);
    }

    //Roditelj
    shmem = (char *)shmat(memid, NULL, 0);
    turn = 1, procAdone = 0, procBdone = 0;
    file = fopen("izlaz.txt", "w");

    do
    {
        semop(writeid, &lock, 1); //blokira writeid

        if ((!procAdone) && (!procBdone)) //ako nijedan nije stigao do kraja
            turn = (turn + 1) % 2;

        if (shmem[0] == 0) //procitan je kraj mora da se odredi koji od procesa A ili B je zavrsio
        {
            if ((!turn && !procAdone) || procBdone)
                procAdone = 1; //ukoliko je procB vec zavrsio ili procA nije zavrsio a procitao je kraj i red je na B

            else
                procBdone = 1;
        }
        else
            fprintf(file, "%s", shmem);

        if (turn==0)
            semop(procbid, &unlock, 1);
        else
            semop(procaid, &unlock, 1);

    } while (!(procAdone && procBdone)); //dok oba ne zavrse

    fclose(file);
    wait(NULL);
    wait(NULL);
    shmdt(shmem);
    semctl(procbid, 0, IPC_RMID, 0);
    semctl(procaid, 0, IPC_RMID, 0);
    shmctl(memid, IPC_RMID, 0);
}