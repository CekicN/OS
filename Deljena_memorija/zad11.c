/*Koriscenjem programskog jezika C napisati Linux program koji se deli na dva procesa.
Proces roditelj sa standardnig ulaza cita liniju teksta i koriscenjem deljene memorije procitane
linije teksta prosledjuje procesu detetu.Proces dete stampa na standardnom izlazu linije teksta
koje prihvata iz deljene memorije.Obezbediti da proces dete stampa linije teksta onim 
redosledom kako se javljaju u tekstualnoj datoteci.Ovaj proces se ponavlja sve dok korisnik na
standardnom ulazu ne unese rec "KRAJ".*/

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
	char* shm_ptr;
	if (fork() != 0)
	{
		char line[80];
		shm_id = shmget(MEM_KEY, 80*sizeof(char), IPC_CREAT | 0666);
		shm_ptr = shmat(shm_id, NULL, 0);
		while(strcmp(line, "KRAJ") != 0)
		{
			semop(procaid, &sem_lock, 1);

			printf("Unesite string: ");
			gets(line);
			memcpy(shm_ptr, line, 80);

			semop(procbid, &sem_unlock, 1);
		}

		wait(NULL);

	}
	else
	{
		char line[80];
		shm_id = shmget(MEM_KEY, 80*sizeof(char), IPC_CREAT | 0666);
		shm_ptr = shmat(shm_id, NULL, 0);

		while(strcmp(line, "KRAJ") != 0)
		{
			semop(procbid, &sem_lock, 1);

			memcpy(line, shm_ptr, 80);
			printf("Iz deljene memorije procitano: %s\n", line);
			fflush(stdout);

			semop(procaid, &sem_unlock, 1);
		}

	}
		shmdt(shm_ptr);
		shmctl(shm_id, IPC_RMID, NULL);
		semctl(procaid, 0, IPC_RMID, NULL);
		semctl(procbid, 0, IPC_RMID, NULL);
}