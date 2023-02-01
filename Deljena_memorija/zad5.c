/*Koriscenjem programskog jezika C kreirati program koji se deli u dva procesa. Proces roditelj cita sa tastature stringove i koristeci deljenu memoriju ih salje
procesu detetu. Proces dete primljene stringove upisuje u datoteku stringovi.txt. Kada korisnik unese string "KRAJ" komunikacija se prekida. */

#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<string.h>

#define PRVI_KEY 10001
#define DRUGI_KEY 10002
#define MEM_KEY 10004
#define N 1024

union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
    struct seminfo *__buf;
    void* pad;

};

int main()
{
    int sem1, sem2, memid;
    int i, j;
    FILE* f;
    char* shmem;
    struct sembuf lock={0,-1,0};
    struct sembuf unlock={0,1,0};
    union semun opts;
    sem1=semget(PRVI_KEY, 1, 0666 | IPC_CREAT);
    sem2=semget(DRUGI_KEY, 1, 0666 | IPC_CREAT);
    opts.val=1;
    semctl(sem1, 0 , SETVAL, opts);
    opts.val=0;
    semctl(sem2, 0, SETVAL, opts);

    if (fork()!=0)
    {
        char line[80];
        int shmid=shmget(MEM_KEY, 80*sizeof(char), IPC_CREAT | 0666);
        char* shmptr=shmat(shmid, NULL, 0);
        while (strcmp(line, "KRAJ")!=0)
        {
            semop(sem1, &lock, 1);
            printf("Unesite string: ");
            gets(line);
            memcpy(shmptr, line, 80);
            semop(sem2, &unlock, 1);
        }
        shmdt(shmptr);
        int status;
        wait(&status);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(sem1, 0, IPC_RMID, NULL);
        semctl(sem2, 0, IPC_RMID, NULL);

    }
    else
    {
        char line[80];
        int shmid=shmget(MEM_KEY, 80*sizeof(char), IPC_CREAT | 0666);
        char* shmptr=shmat(shmid, NULL, 0);
        while (strcmp(line, "KRAJ")!=0)
        {
            semop(sem2, &lock, 1);
            memcpy(line, shmptr, 80);
            printf("Procitano iz memorije: %s\n", line);
            fflush(stdout);
            semop(sem1, &unlock, 1);
        }
        shmdt(shmptr);
    }

}