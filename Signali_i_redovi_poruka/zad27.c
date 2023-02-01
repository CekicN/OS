/*Korišćenjem programskog jezika C kreirati dva Linux procesa koji komuniciraju korišćenjem deljive
memorije i signala. Prvi proces očitava poruku sa standardnog ulaza, smešta je u deljivu memoriju i o
tome obaveštava drugi proces slanjem signala SIGUSR1. Drugi proces po prijemu signala, očitava
sadržaj deljive memorije i štampa ga na standardnom izlazu. Uzeti da je maksimalana veličina poruke
koja može da se pošalje 1024B.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#define MEM_KEY 10004
#define N 1024
int main()
{
 int memid; //reference semafora i deljive memorije
 int i, pid;
 char * shmem; //pokazivac na niz bajtova koji se koristi za mapiranje deljive memorije
 //kreiramo deljenu memoriju
 memid = shmget(MEM_KEY, N * sizeof(char), IPC_CREAT | 0666);
 pid = fork();
 if (pid < 0)
 {
 printf("Doslo je do greske prilikom kreiranja novog procesa.\n");
 exit(-1);
 }
 if (pid == 0)
 {
 execl("II-1-2a", "II-1-2a", NULL); 
 printf("Doslo je do greske prilikom pozivanja izvrsne datoteke.\n");
 exit(-1);
 }

 //roditeljski proces
 //mapiramo deljenu memoriju
 shmem = (char *)shmat(memid, NULL, 0);
 for (i = 0; i < 100; i++)
 {
 gets(shmem);
 kill(pid, SIGUSR1);
 sleep(5);

 }
 //izbacujemo deljenu memoriju iz adresnog prostora
 shmdt(shmem);
 kill(pid, SIGKILL);
 shmctl(memid, IPC_RMID, 0);
 return 0;

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#define MEM_KEY 10004
#define N 1024
void usr_signal()
{
 int memid; //reference semafora i deljive memorije
 char * shmem; //pokazivac na niz bajtova koji se koristi za mapiranje deljive memorije
 //kreiramo deljenu memoriju
 memid = shmget(MEM_KEY, N * sizeof(char), IPC_CREAT | 0666);
 //mapiramo deljenu memoriju
 shmem = (char *)shmat(memid, NULL, SHM_RDONLY);
 printf("%s\n", shmem);
 //izbacujemo deljenu memoriju iz adresnog prostora
 shmdt(shmem);

}
int main()
{
 signal(SIGUSR1, usr_signal);
 while(1)
 pause();
 return 0;
}