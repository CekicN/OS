/*Koriscenjem programskog jezika C kreirati 2 Linux procesa. Prvi proces kreira u deljenom memorijskom segmentu matricu dimenzija NxM.
Drugi proces u svakoj od kolona matrice pronalazi maksimalni element i stampa ga na ekranu. */


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
#define MEM_KEY 10004
#define N 4
#define M 5

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
    int i, j;
    int semid;
    union semun semopts;
    struct sembuf sem_lock = {0,-1,0};
    struct sembuf sem_unlock= {0, 1, 0};
    semid=semget((key_t)PRVI_KEY, 1, 0666 | IPC_CREAT);
    semopts.val=0;
    semctl(semid, 0, SETVAL, semopts);

    if (fork()!=0)
    {
        int shmid=shmget(MEM_KEY, M*N*sizeof(int), IPC_CREAT | 0666);
        int* shmptr=shmat(shmid, NULL, 0);
        for(int i=0; i<N; i++)
        {
            for (int j=0; j<M; j++)
            {
                shmptr[i*M+j]=rand()%100;
                printf("%d ", shmptr[i*M+j]);
            }
            printf("\n");
        }
        semop(semid, &sem_unlock, 1);
        shmdt(shmptr);
        int status;
        wait(&status);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID, NULL);

    }

    else
    {
        int shmid=shmget(MEM_KEY, N*M*sizeof(int), IPC_CREAT | 0666);
        int* shmptr=shmat(shmid, NULL, 0);
        for (int i=0; i<N; i++)
        {
            int maxel=0;
            for (int j=0; j<M; j++)
            {
                if (shmptr[i*M+j]>maxel)
                maxel=shmptr[i*M+j];
            }
            printf("Maksimalni element %d-te kolone je %d\n", i, maxel);
        }
        shmdt(shmptr);
    }
}