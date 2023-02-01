/*
1. Korišćenjem programskog jezika C napisati UNIX/Linux program koji se deli u dva procesa a kreirani procesi komuniciraju deljenom memorijom. 
   Roditeljski proces omogućava korisniku da unese string sa tastature i smešta ga u deljivu memoriju. 
   Proces dete čita string iz deljive memorije i upisuje ga u tekstualnu datoteku izlaz.txt. 
   String koji se unosi je maksimalne dužine 80 karaktera. 
   Proces roditelj posle unošenja stringa sa tastature čeka proizvoljan broj sekundi pre upisa stringa u deljenu memoriju.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

#define PROCA_KEY 10101
#define PROCB_KEY 10102
#define MEM_KEY 10001
#define N 80

union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
    struct seminfo *__buf;
    void *__pad;
};

int main()
{
    int procaid, procbid, memid;
    int *shmem;
    int test;
    int pid;
    //1. Operacije nad semaforima
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};

    //2. Instanca unije
    union semun opts;

    //3. Kreiranje i inicijalizacija semafora

    procaid = semget(PROCA_KEY, 1, IPC_CREAT | 0666);
    if (procaid < 0)
    {
        printf("Greska prilikom kreiranja prvog semafora!\n");
        exit(-1);
    }
    opts.val = 1;

    test = semctl(procaid, 0, SETVAL, opts);
    if (test < 0)
    {
        printf("Greska prilikom inicijalizacije prvog semafora!\n");
        exit(-1);
    }

    //Ponovljeni postupak za drugi semafor

    procbid = semget(PROCB_KEY, 1, IPC_CREAT | 0666);
    if (procbid < 0)
    {
        printf("Greska prilikom kreiranja drugog semafora!\n");
        exit(-1);
    }
    opts.val = 0;

    test = semctl(procbid, 0, SETVAL, opts);
    if (test < 0)
    {
        printf("Greska prilikom inicijalizacije drugog semafora!\n");
        exit(-1);
    }

    //Deljena memorija

    memid = shmget(MEM_KEY, N * sizeof(char), IPC_CREAT | 0666);
    if (memid < 0)
    {
        printf("Greska prilikom kreiranja deljene memorije!\n");
        exit(-1);
    }

    //Konacno zadatak
    pid=fork();

    if(pid!=0)
    {   
        char linija[N];
        shmem=shmat(memid,NULL,0); // Dodeljujemo deljenu memoriju

        semop(procaid,&lock,1);//Za sinhronizaciju

        printf("Unesite neku recenicu:\n");
        gets(linija);

        sleep(rand()%5);
        memcpy(shmem,linija,N); //Upis u deljenu memoriju

        semop(procbid,&unlock,1); //Za sinhronizaciju

        shmdt(shmem); //Oduzimamo deljenu memoriju

        int status;
        wait(status);
        semctl(procaid,0,IPC_RMID,NULL);
        semctl(procbid,0,IPC_RMID,NULL);
        shmctl(shmem,IPC_RMID,NULL);
        

    }
    else
    {
        FILE *f;
        char linija[N];
        shmem= shmat(memid, NULL, 0);
 
        semop(procbid, &lock, 1);
        memcpy(linija, shmem, N);
        printf("Iz deljene memorije je procitano: %s\n", linija);
        fflush(stdout);

        f=fopen("izlaz.txt", "w");
        fprintf(f, "%s", linija);

        fclose(f);
        semop(procaid, &unlock, 1);

        shmdt(shmem);

    }
    
    return 0;
}