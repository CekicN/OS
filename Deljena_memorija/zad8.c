/*2. Korišćenjem programskog jezika C napisati UNIX/Linux program koji kreira jedan dodatni proces.
Ova dva procesa komuniciraju deljenom memorijom veličine 2 int broja. 
Roditeljski proces učitava jedan broj sa tastature a zatim taj broj i njegovu trostruku vrednost upisuje u deljivu memoriju . 
Proces dete čita ove brojeve iz deljive memorije i na ekranu štampa njihov zbir. Ovaj ciklus se ponavlja sve dok se sa tastature ne unese KRAJ.*/

/*KAKO UPISATI KRAJ KAO DVA INTEGERA - ODABEREM SAMA NEKE VREDNOSTI NPR 1 1 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>

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
    int pid;

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};

    union semun opts;

    //Prvi semafor
    procaid = semget(PROCA_KEY, 1, IPC_CREAT | 0666);
    opts.val = 1;
    semctl(procaid, 0, SETVAL, opts);

    //Drugi semafor
    procbid = semget(PROCB_KEY, 1, IPC_CREAT | 0666);
    opts.val = 0;
    semctl(procbid, 0, SETVAL, opts);

    //Deljiva memorija
    memid = shmget(MEM_KEY, 2 * sizeof(int), IPC_CREAT | 0666);

    //Zadatak

    pid = fork();

    if (pid != 0)
    {
        int *shmem = shmat(memid, NULL, 0); //mapiranje deljive memorije na roditelja

        char linija[N];

        while (strcmp(shmem, "KRAJ") != 0)
        {
            semop(procaid, &lock, 1);

            if (strcmp(shmem,"KRAJ")==0)
            {
                printf("Zavrsavam..\n");
                return 0;
            }
            printf("Unesite broj:\n");
            gets(linija);

            if (strcmp(linija, "KRAJ") != 0)
            {
                int broj = atoi(linija);
                shmem[0] = broj;
                shmem[1] = 3 * broj;
            }
            else //kod za KRAJ
            {
                shmem[0] = 0;
                shmem[1] = 0;
            }

            semop(procbid, &unlock, 1);
        }

        shmdt(shmem); //Oduzimamo deljenu memoriju

        
        wait(NULL);

        semctl(procaid, 0, IPC_RMID, NULL);
        semctl(procbid, 0, IPC_RMID, NULL);
        shmctl(shmem, IPC_RMID, NULL);
    }
    else
    {
        int *shmem = shmat(memid, NULL, 0);
        char provera[N];

        while (strcmp(provera, "KRAJ") != 0)
        {
            semop(procbid, &lock, 1);
            int x = shmem[0];
            int y = shmem[1];

            if (x == 0 && y == 0)
            {
                strcpy(provera, "KRAJ");
                memcpy(shmem,provera,N);
                printf("Detektovan je kraj!\n"); // lepo mi ovde ulazi program
                fflush(stdout);
            }
            else
            {
                strcpy(provera,"nijeKRAJ");
                int zbir=x+y;
                printf("Prvi broj %d\nDrugi broj %d\nZbir %d\n",x,y,zbir);
                fflush(stdout);
            }
            semop(procaid, &unlock, 1);
        }

        shmdt(shmem);
    }

    return 0;
}


