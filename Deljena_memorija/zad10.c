/*
Koriščenjem programskog jezika C napisati Linux/UNIX program koji kao argument komandne linije prihvata naziv tekstualne datoteke u tekućem direktorijumu 
koja sadrži više linija teksta. Program se potom deli u dva procesa pri čemu proces roditelj otvara tekstualnu datoteku i čita iz nje liniju po liniju  
teksta i korišćenjem deljene memorije pročitanu liniju prosleđuje procesu detetu. 
Proces dete štampa na standardnom izlazu linije teksta koje čita iz deljene memorije u ispravnom redosledu
*/
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

int main(int argc, char *argv[])
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

    char *dirname = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
    strcpy(dirname, argv[1]);

    pid = fork();
    if (pid != 0)
    {
        char* shmem = shmat(memid, NULL, 0); //mapiranje deljive memorije na roditelja
        FILE *f = fopen(dirname, "r");
        int p = 1;

        do
        {
            semop(procaid, &lock, 1);

            if (fgets(shmem, N, f) == NULL)
            {
                p = 0;
                shmem[0] = 0; // kod za eof
            }

            semop(procbid, &unlock, 1);

        } while (p);

        fclose(f);
        shmdt(shmem);
        wait(NULL);
        semctl(procaid, 0, IPC_RMID, NULL);
        semctl(procbid, 0, IPC_RMID, NULL);
        shmctl(memid, IPC_RMID, NULL);
    }
    else
    {
        int *shmem = shmat(memid, NULL, 0);
        int procAdone = 0; //inicijalno A nije zavrsio.
        char linija[N];
        int i=0;

        do
        {
            semop(procbid, &lock, 1);
            memcpy(linija,shmem,N);
            if(linija[0]==0)
            {
                procAdone=1;
                printf("Procitan je EOF\n");
            }
            else
            {
                printf("Procitana %d . linija: %s",i++,linija);
            }

            semop(procaid, &unlock, 1);
            
        } while (procAdone == 0); //dok proc A ne zavrsi
    }
}
