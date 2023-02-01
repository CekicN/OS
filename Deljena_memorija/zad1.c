/*Koriscenjem programskog jezika C napisati Linux program koji se deli u dva procesa (proizvodjac-potrosac), koji komuniciraju koristeci ogranicen kruzni bafer
u deljivoj memoriji, velicine 10 celih brojeva. Ovi procesi sinhronizuju svoje aktivnosti koriscenjem semafora.  */

#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>

#define FULL_KEY 100001
#define EMPTY_KEY 100002
#define MUTEX_KEY 100003
#define MEM_KEY 100004
#define N 10

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
    int fullid, emptyid, mutexid;
    int memid;
    int* shmem;
    int i;
    struct sembuf lock={0,-1,0};
    struct sembuf unlock={0,1,0};
    union semun opts;

    fullid=semget(FULL_KEY, 1, IPC_CREAT | 0666);
    if (fullid<0)
    {
        printf("Greska");
        exit(1);
    }
    opts.val=0;
    if (semctl(fullid, 0, SETVAL, opts)<0)
    {
        printf("Greska");
        exit(1);
    }

    emptyid=semget(EMPTY_KEY, 1, IPC_CREAT | 0666);
    if (emptyid<0)
    {
        printf("Greska");
        exit(1);
    }
    opts.val=N;
    if (semctl(emptyid, 0, SETVAL, opts)<0)
    {
        printf("Greska");
        exit(1);
    }

    mutexid=semget(MUTEX_KEY, 1, IPC_CREAT | 0666);
    if (mutexid<0)
    {
        printf("Greska");
        exit(1);
    }
    opts.val=1;
    if (semctl(mutexid, 0, SETVAL, opts)<0)
    {
        printf("Greska");
        exit(1);
    }

    memid=shmget(MEM_KEY, N*sizeof(int), IPC_CREAT | 0666);
    if (memid<0)
    {
        printf("Greska pri kreiranju deljene memorije");
        exit(1);
    }

    if (fork()==0)
    {
        int next_write=0;
        shmem=shmat(memid, NULL, 0);
        if (shmem==NULL)
        {
            printf("Greska prilikom mapiranja deljene memorije");
            exit(1);
        }

        for (int i=0; i<20; i++)
        {
            semop(emptyid, &lock, 1);
            semop(mutexid, &lock, 1);
            int p=rand()%100;
            printf("Generisani broj je: %d\n", p);
            shmem[next_write]=p;
            next_write=(next_write+1)%N;
            semop(mutexid, &unlock, 1);
            semop(fullid, &unlock, 1);
            sleep(1);
        }
        shmdt(shmem);
    }
    else
    {
        int next_read=0;
        shmem=shmat(memid, NULL, 0);
        if (shmem==NULL)
        {
            printf("Greska prilikom mapiranja deljene memorije");
            exit(1);
        }
        for (int i=0; i<20; i++)
        {
            semop(fullid, &lock, 1);
            semop(mutexid, &lock, 1);
            int p=shmem[next_read];
            printf("Procitani broj je: %d\n", p);
            next_read=(next_read+1)%N;
            semop(mutexid, &unlock, 1);
            semop(emptyid, &unlock, 1);
            sleep(2);
        }

        semctl(mutexid, 0, IPC_RMID, 0);
        semctl(fullid, 0, IPC_RMID, 0);
        semctl(emptyid, 0, IPC_RMID, 0);
        shmctl(memid, IPC_RMID, 0);

    }
    return 0;

}
