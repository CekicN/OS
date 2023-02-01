#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<string.h>
#include<sys/wait.h>
union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
    struct seminfo *_buf;
    void * _pad;
};

int main()
{
    int s1,s2;
    int memid;
    char *shmem;
    int i;
    struct sembuf lock = {0,-1,0};
    struct sembuf unlock = {0,1,0};

    union semun opts;

    s1 = semget(10000, 1, IPC_CREAT | 0666);
    s2 = semget(10001, 1, IPC_CREAT | 0666);

    opts.val = 1;
    semctl(s1, 0, SETVAL, opts);
    opts.val = 0;
    semctl(s2, 0, SETVAL, opts);

    memid = shmget(10002, 100, IPC_CREAT | 0666);
    if(fork() != 0)
    {
        //roditelj
        shmem = (char*)shmat(memid, NULL, 0);
        char buf[100];
        do
        {
            gets(buf);
            semop(s1,&lock, 1);
            strcpy(shmem, buf);
            semop(s2, &unlock, 1);
        }while(strcmp(buf, "KRAJ") != 0);

        wait(NULL);
        shmdt(shmem);

        semctl(s1, 0, IPC_RMID, 0);
        semctl(s2, 0, IPC_RMID, 0);
        semctl(memid, 0, IPC_RMID, 0);

    }
    else
    {
        //dete
        shmem = (char*)shmat(memid, NULL, 0);
        char buf[100];
        do
        {
            semop(s2,&lock, 1);
            //
            strcpy(buf, shmem);
            printf("%s\n", buf);
            //
            semop(s1, &unlock, 1);
        }while(strcmp(buf, "KRAJ") != 0);

        shmdt(shmem);
    }

    return 0;
}
