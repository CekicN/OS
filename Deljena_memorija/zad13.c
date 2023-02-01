/*
Koriscenjem programskog jezika C kreirati 2 LINUX procesa koji stogo naizmenivcno komunicarau
putem deljivog memorijskog segmenta dimenzija 10 celobrojnih vresnosti. Prvi proces generise
10 slucajnih celobrojnih vrednosti i upisuje ih u memoriju. Drugi proces cita vresnosti i 
stampa na standardni izlaz.

*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define MEM_KEY 1122
#define PROC_A_KEY 10101
#define PROC_B_KEY 10102

union semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int main()
{
	int i, procaid, procbid;

	union semun semopts;
	struct sembuf sem_lock = {0, -1, 0};
	struct sembuf sem_unlock = {0, 1, 0};

	procaid = semget((key_t)PROC_A_KEY, 1, 0666 | IPC_CREAT);
	procbid = semget((key_t)PROC_B_KEY, 1, 0666 | IPC_CREAT);

	semopts.val = 1;
	semctl(procaid, 0, SETVAL, semopts);
	semopts.val = 0;
	semctl(procbid, 0, SETVAL, semopts);
	int shm_id;
	int* shmem;
	if (fork() != 0)
	{
		shm_id = shmget(MEM_KEY, 10*sizeof(int), IPC_CREAT | 0666);
        shmem = (int*)shmat(shm_id, NULL, 0);

		semop(procaid, &sem_lock, 1);
        int br;
		for(int i=0; i<10;i++){
            shmem[i] = random() % 100;
            printf("Upisano u deljenu memoriju: %d\n", shmem[i]);
            fflush(stdout);
		}
		semop(procbid, &sem_unlock, 1);

		wait(NULL);

	}
	else
	{
		shm_id = shmget(MEM_KEY, 10*sizeof(int), IPC_CREAT | 0666);
		shmem = shmat(shm_id, NULL, 0);

		semop(procbid, &sem_lock, 1);

        for(int i =0; i<10;i++){
    		printf("Iz deljene memorije procitano: %d\n", shmem[i]);
			fflush(stdout);
        }

		semop(procaid, &sem_unlock, 1);
		

	}
		shmdt(shmem);
		shmctl(shm_id, IPC_RMID, NULL);
		semctl(procaid, 0, IPC_RMID, NULL);
		semctl(procbid, 0, IPC_RMID, NULL);
}