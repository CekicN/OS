/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji se deli u dva procesa. Prvi
process periodično (100 puta) generiše dve slučajne celobrojne vrednosti. Drugi process na ekranu
štampa generisane brojeve, njihovu sumu i ukupnu sumu svih prethodno generisanih brojeva. Ova
dva procesa komuniciraju deljenom memorijom odgovarajude veličine. Sinhronizovati rad ova dva
procesa korišdenjem semafora. */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>


#define SEM1_KEY 10010
#define SEM2_KEY 10011
#define SHM_KEY 10012

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  array;  /* Array for GETALL, SETALL */
    struct seminfo  __buf;  /* Buffer for IPC_INFO
    (Linux-specific) */
};


int main()
{
    
    int prviSemID = semget((key_t)SEM1_KEY,1,0666 | IPC_CREAT);
    int drugiSemID = semget((key_t)SEM2_KEY,1,0666 | IPC_CREAT);
    int shMemoryID = shmget((key_t)SHM_KEY,2* sizeof(int),0666 | IPC_CREAT);
    
    struct sembuf lock = {0,-1,0};
    struct sembuf unlock = {0,1,0};
    
    union semun sval;
    
    sval.val = 1;
    semctl(prviSemID,0,SETVAL,sval);
    sval.val = 0;
    semctl(drugiSemID,0,SETVAL,sval);
    
    
    if(fork() != 0)
    {
        int i;
        int* shm = (int*)shmat(shMemoryID,NULL,0);
        
        for(i=0;i < 10;i++)
        {
            semop(prviSemID,&lock,1);
            shm[0] = rand() % 100;
            shm[1] = rand() % 100;
            printf("Roditelj generisao %d\t%d\n",shm[0],shm[1]);
            semop(drugiSemID,&unlock,1);
            sleep(2);
        }
        shmdt(shm);
        
        wait(NULL);
        semctl(prviSemID,0,IPC_RMID,0);
        semctl(drugiSemID,0,IPC_RMID,0);
        shmctl(shMemoryID,0,IPC_RMID);
        
    }
    else
    {
        int i;
        int ukupnaSuma = 0;
        int* shm = (int*)shmat(shMemoryID,NULL,0);
        for(i=0;i<10;i++)
        {
            semop(drugiSemID,&lock,1);
            printf("Dete prima %d\t%d\n",shm[0],shm[1]);
            printf("Suma je %d\n",shm[0] + shm[1]);
            printf("Ukupna suma do sada je %d\n", ukupnaSuma += shm[0] + shm[1]);
            
            semop(prviSemID,&unlock,1);
            sleep(2);
        }
        shmdt(shm);
    }
    
    
    
}