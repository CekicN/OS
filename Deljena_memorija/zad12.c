/*Koriscenjem programskog jezika C napisati Linux program koji omogucava da dva procesa komuniciraju i sinhronizuju svoje izvrsavanje
koriscenjem signala i deljene memorije. Prvi proces po slucajnom principu kreira 1024 slucajnih vrednosti i smesta ih u deljeni memorijski prostor.
Kada izgenerise brojeve, obavestava o tome drugi proces, slanjem signala SIGUSR1 i pauzira svoje izvrsavanje. Kada drugi proces primi signal, 
on sadrzaj deljenog memorijskog prostora stampa na standardni izlaz, salje prvom procesu signal SIGUSR2 i nakon toga pauzira svoje izvrsavanje. 
Po prijemu signala SIGUSR2, prvi proces ponovo startuje svoje izvrsenje. Postupak se ponavlja 4096 puta. Nakon toga se procesi zavrsavaju. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/sem.h>
#include <signal.h>
int memid;
int* shmem;
void obradaJedan()
{
    shmem = (int*)shmat(memid, NULL, 0);
    int i;
    for(i = 0; i < 10; i++)
    {
        shmem[i] = random() % 100;
    }
    shmdt(shmem);
}
void obradaDva()
{
    shmem = (int*)shmat(memid, NULL, 0);
    int i;
    for(i = 0; i < 10; i++)
    {
        printf("Procitan broj: %d\n", shmem[i]);
    }
    shmdt(shmem);
}
int main()
{
    int pid;
    memid = shmget(15013, 10 * sizeof(int), IPC_CREAT | 0666);
    if((pid = fork()) == 0)
    {
        signal(SIGUSR1, obradaJedan);
        pause();
        sleep(1);
        kill(pid, SIGUSR2);
    }
    else
    {
        sleep(1);
        kill(pid, SIGUSR1);
        signal(SIGUSR2, obradaDva);
        pause();
    }
    return 0;
}