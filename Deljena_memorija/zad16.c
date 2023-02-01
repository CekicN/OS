/*Koriscenjem programskog jezika C kreirati dva Linux/UNIX procesa. Prvi proces kreira u deljenom memorijskom
segmentu matricu dimenzija MxN popunjenu slučajnim vrednostima. Drugi proces u svakoj koloni
matrice pronalazi maksimalni element i njegovu vrednost štampa na ekranu. Treći proces u svakoj vrsti
nalazi maksimalni element i njegovu vrednost štampa na ekranu.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/ipc.h>
#define N 4
#define M 5
#define MEM_KEY 1122
#define SEM_KEY 3344
#define SEM_KEY1 3345
union semun{
 int val;
 struct semid_ds *buf;
 ushort *array;
};
int main()
{
 int i, j;
 int semid, semid1;
 union semun semopts;
 struct sembuf sem_lock = {0, -1, NULL};
 struct sembuf sem_unlock = {0, 1, NULL};
 semid = semget((key_t)SEM_KEY, 1, 0666 | IPC_CREAT);
 semid1 = semget((key_t)SEM_KEY1, 1, 0666 | IPC_CREAT);
 semopts.val = 0;
 semctl(semid, 0, SETVAL, semopts);
 semctl(semid1, 0, SETVAL, semopts);
 if (fork() != 0)
 {
 int shm_id = shmget(MEM_KEY, M*N*sizeof(int), IPC_CREAT | 0666);
 int* shm_ptr = shmat(shm_id, NULL, 0);
 for(i=0 ; i<N ; i++)
 for(j=0 ; j<M ; j++)
{
 shm_ptr[i*M+j] = rand() % 100;
 printf("Upisujem na poziciju %d,%d broj %d\n", i, j, shm_ptr[i*M+j]);
 }
 semop(semid, &sem_unlock, 1);
 shmdt(shm_ptr);
 int status;
wait(&status);
 shmctl(shm_id, IPC_RMID, NULL);
}
else if (fork() != 0) {
 int shm_id = shmget(MEM_KEY, M*N*sizeof(int), IPC_CREAT | 0666);
 int* shm_ptr = shmat(shm_id, NULL, 0);
 semop(semid, &sem_lock, 1);
 for(i=0 ; i<N ; i++)
 {
 int maxel = 0;
 for(j=0 ; j<M ; j++)
 {
 if (shm_ptr[i*M+j] > maxel)
 maxel = shm_ptr[i*M+j];
 }
 printf("Maksimalni element kolone %d je: %d\n", i, maxel);
 }
 shmdt(shm_ptr);
semop(semid1, &sem_unlock, 1);
 int status;
 wait(&status);
 }
else {
 int shm_id = shmget(MEM_KEY, M*N*sizeof(int), IPC_CREAT | 0666);
 int* shm_ptr = shmat(shm_id, NULL, 0);
 semop(semid1, &sem_lock, 1);
 for(j=0 ; j<M ; j++)
 {
 int maxel = 0;
 for(i=0 ; i<N ; i++)
{
 if (shm_ptr[i*M+j] > maxel)
 maxel = shm_ptr[i*M+j];
 }
 printf("Maksimalni element vrste %d je: %d\n", j, maxel);
 }
 shmdt(shm_ptr);
 }