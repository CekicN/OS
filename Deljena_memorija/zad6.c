/*Koriscenjem programskog jezika C napisati program koji kopira zadatu datoteku. Prvi argument komandne linije je naziv originalne datoteke, a drugi argument je
naziv nove datoteke, koja ce biti kopija prve. Kopiranje se obavlja tako sto se program deli u dva procesa, prvi proces cita odredjeni broj bajtova iz datoteke i salje
ih drugom procesu. Drugi proces bajtove iz deljene memorije upisuje u novu datoteku. */

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

int main(int argc, char* argv[])
{
    int sem1, sem2, memid;
    int i, j;
    FILE* f1=fopen(argv[1], "r");
    FILE* f2=fopen(argv[2], "a");
    char* shmem;
    struct sembuf lock={0,-1,0};
    struct sembuf unlock={0,1,0};
    union semun opts;
    sem1=semget(PRVI_KEY, 1, IPC_CREAT | 0666);
    opts.val=1;
    semctl(sem1, 0, SETVAL, NULL);
    sem2=semget(DRUGI_KEY, 1, IPC_CREAT | 0666);
    opts.val=0;
    semctl(sem2, 0, SETVAL, NULL);

    if (fork()!=0)
    {
        char line[80];
        int shmid=shmget(MEM_KEY, 80*sizeof(char), IPC_CREAT | 0666);
        shmem=shmat(shmid, 0, NULL);
        while (!feof(f1))
        {
            semop(sem1, &lock, 1);
            fgets(line, 80, f1);
            memcpy(shmem, line, 80);
            printf("Procitano iz fajla: %s\n", line);
            fflush(stdout);
            semop(sem2, &unlock, 1);
        }
        shmdt(shmem);
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
        shmem=shmat(shmid, 0, NULL);
        while (!feof(f1))
        {
            semop(sem2, &lock, 1);
            //fgets(line, 80, f1);
            memcpy(line, shmem, 80);
            printf("Procitano iz memorije: %s\n", line);
            fflush(stdout);
            semop(sem1, &unlock, 1);
        }
        shmdt(shmem);
       
    }
}
