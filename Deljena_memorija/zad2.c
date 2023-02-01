/*Koriscenjem programskog jezika C kreirati tri Linux procesa koji za medjusobnu komunikaciju koriste deljenu memoriju velicine 1024 bajtova. 
Prvi proces popunjava prvih 512 bajtova deljive memorije slucajno izabranim slovima u opsegu a-z. Nakon toga, drugi proces popunjava poslednjih 512 bajtova
deljene memorije proizvoljno izabranim ciframa. Posto i drugi proces zavrsi generisanje podataka, treci proces kompletan sadrzaj deljene memorije upisuje u datoteku. 
Ova sekvenca akcija se periodicno obavlja na svakih 15s. */

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
#define TRECI_KEY 10003
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
    int sem1, sem2, sem3, memid;
    int i, j;
    FILE* f;
    char* shmem;
    struct sembuf lock={0,-1,0};
    struct sembuf unlock={0,1,0};
    union semun opts;

    sem1=semget(PRVI_KEY, 1, IPC_CREAT | 0666);
    if (sem1<0)
    {
        printf("Greska");
        exit(1);
    }
    opts.val=1;
    if (semctl(sem1, 0, SETVAL, opts)<0)
    {
        printf("Greska");
        exit(1);
    }

    sem2=semget(DRUGI_KEY, 1, IPC_CREAT | 0666);
    if (sem2<0)
    {
        printf("Greska");
        exit(1);
    }
    opts.val=0;
    if (semctl(sem2, 0, SETVAL, opts)<0)
    {
        printf("Greska");
        exit(1);
    }

    sem3=semget(TRECI_KEY, 1, IPC_CREAT | 0666);
    if (sem3<0)
    {
        printf("Greska");
        exit(1);
    }
    opts.val=0;
    if (semctl(sem3, 0, SETVAL, opts)<0)
    {
        printf("Greska");
        exit(1);
    }

    memid=shmget(MEM_KEY, N, IPC_CREAT | 0666);
    if (memid<0)
    {
        printf("Greska pri kreiranju deljene memorije");
        exit(1);
    }

    if (fork()==0)
    {
        shmem=(char*)shmat(memid, NULL, 0);
        for (int j=0; j<10; j++)
        
        {
            semop(sem1, &lock, 1);
            for (int i=0; i<N/2; i++)
            {
                
                char p='a'+rand()%26;
                shmem[i]=p;
                
            }
            semop(sem2, &unlock, 1);
        }
        shmdt(shmem);
        sleep(2);
    }

    if (fork()==0)
    {
        shmem=(char*)shmat(memid, NULL, 0);
        for (int j=0; j<10; j++)
        {
            semop(sem2, &lock, 1);
    
            for (int i=N/2; i<N; i++)
            {
                
                char p='0'+rand()%10;
                shmem[i]=p;
                
            }
            semop(sem3, &unlock, 1);
        }
        shmdt(shmem);
        sleep(2);
    }

    shmem=(char*)shmat(memid, NULL, SHM_RDONLY);
    semop(sem3, &lock, 1);
    for (int i=0; i<10; i++)
    {
        
        char buff[N+1];
        strncpy(buff, shmem, N);
        buff[N]='\0';
        f=fopen("fajl.txt", "a");
        fprintf(f, "%s\n", buff);
        fclose(f);
        
    }
    semop(sem1, &unlock, 1);
    wait(NULL);

    semctl(sem1, 0, IPC_RMID, opts);
    semctl(sem2, 0, IPC_RMID, opts);
    semctl(sem3, 0, IPC_RMID, opts);
    shmctl(memid, IPC_RMID, 0);
    

}