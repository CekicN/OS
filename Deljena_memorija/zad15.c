/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji se deli u dva procesa. Prvi
process periodično (100 puta) generiše dve slučajne celobrojne vrednosti. Drugi process na ekranu
štampa generisane brojeve, njihovu sumu i ukupnu sumu svih prethodno generisanih brojeva. Ova
dva procesa komuniciraju deljenom memorijom odgovarajude veličine. Sinhronizovati rad ova dva
procesa korišdenjem semafora. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#define MEM_KEY 1122
#define PROC_A_KEY 10101
#define PROC_B_KEY 10102
union semun{
int val;
struct semid_ds *buf;
ushort *array;};
int main(int argc, char* argv[])
{
int i, procaid, procbid;
union semun semopts;
struct sembuf sem_lock = {0, -1, NULL};
struct sembuf sem_unlock = {0, 1, NULL};
procaid = semget((key_t)PROC_A_KEY, 1, 0666 | IPC_CREAT);
procbid = semget((key_t)PROC_B_KEY, 1, 0666 | IPC_CREAT);
semopts.val = 1;
semctl(procaid, 0, SETVAL, semopts);
semopts.val = 0;
semctl(procbid, 0, SETVAL, semopts);
printf("argv[1]: %s, argv[2]: %s\n", argv[1], argv[2]);
if (fork() != 0)
{
int shm_id = shmget(MEM_KEY, 2*sizeof(int), IPC_CREAT | 0666);
int *shm_ptr = shmat(shm_id, NULL, 0);
for(i=0 ; i<100 ; i+=2)
{
semop(procaid, &sem_lock, 1);
shm_ptr[0] = i;
shm_ptr[1] = i+1;
printf("Proces roditelj upisao brojeve %d i %d\n", shm_ptr[0], shm_ptr[1]);
semop(procbid, &sem_unlock, 1);
}
printf("Proces roditelj zavrsio, ceka dete!\n");
fflush(stdout);
shmdt(shm_ptr);
int status;
wait(&status);
shmctl(shm_id, IPC_RMID, NULL);
semctl(procaid, IPC_RMID, NULL);
semctl(procbid, IPC_RMID, NULL);
printf("Proces roditelj sacekao dete, izlazi!\n");
fflush(stdout);
} else {
int shm_id = shmget(MEM_KEY, 2*sizeof(int), IPC_CREAT | 0666);
int *shm_ptr = shmat(shm_id, NULL, 0);
int ukupnasuma = 0;
for(i=0 ; i<50 ; i++)
{
semop(procbid, &sem_lock, 1);
ukupnasuma += shm_ptr[0]+shm_ptr[1];
printf("Suma brojeva %d i %d je %d.\n", shm_ptr[0], shm_ptr[1], shm_ptr[0]+shm_ptr[1]);
semop(procaid, &sem_unlock, 1);
}
shmdt(shm_ptr);
printf("Proces dete zavrsio!\n");
fflush(stdout);
}
}