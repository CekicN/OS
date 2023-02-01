/*
Koriscenjem programskog jezika C napisati Linux program koji ucitava podatke iz tekstualne 
datoteke poruke (red po red) i zatim koriscenjem redova poruka sve parne redove salje procesu
koji konvertuje sva slova u velika,a sve neparne redove salje procesu koji konvertuje sva slova u mala.
Drugi i treci proces konvertovane linije upisuju u datoteku sredjeno.txt,pri cemu treba voditi
racuna da se linije upisuju u pravilnom redosledu.

*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <ctype.h>

#define RED_PORUKA 10001
#define PROC_A_KEY 10101
#define PROC_B_KEY 10102

union semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

struct poruka
{
    long tip;
    char tekst[100];
};

int main()
{
    int procaid, procbid;

	union semun semopts;
	struct sembuf sem_lock = {0, -1, 0};
	struct sembuf sem_unlock = {0, 1, 0};

	procaid = semget((key_t)PROC_A_KEY, 1, 0666 | IPC_CREAT);
	procbid = semget((key_t)PROC_B_KEY, 1, 0666 | IPC_CREAT);

	semopts.val = 1;
	semctl(procaid, 0, SETVAL, semopts);
	semopts.val = 0;
	semctl(procbid, 0, SETVAL, semopts);

    int redid = msgget(RED_PORUKA, IPC_CREAT | 0666);

    if(fork() ==0){
        struct poruka p;
        FILE *f;
        
        
        msgrcv(redid, &p, sizeof(struct poruka), 1, 0);
        do{
    
            for(int i =0; i< strlen(p.tekst);i++){
                p.tekst[i] = toupper(p.tekst[i]);
            }
            semop(procaid, &sem_lock, 1);

                f = fopen("sredjeno.txt", "a+");
                puts(p.tekst);
                fflush(stdout);
                fputs(p.tekst, f);
                fflush(f);
                fclose(f);

		    semop(procbid, &sem_unlock, 1);

            msgrcv(redid, &p, sizeof(struct poruka), 1, 0);

        }while (strcmp(p.tekst, "KRAJ"));
        return 0;

    }
    else if(fork() == 0){
        
        struct poruka p;
        FILE *f;
        msgrcv(redid, &p, sizeof(struct poruka), 2, 0);
        do{

            for(int i =0; i< strlen(p.tekst); i++){
                p.tekst[i] = tolower(p.tekst[i]);
            }

            semop(procbid, &sem_lock, 1);

                f = fopen("sredjeno.txt", "a+");   
                puts(p.tekst);
                fflush(stdout);
                fputs(p.tekst, f);
                fflush(f);
                fclose(f);

            semop(procaid, &sem_unlock, 1);

            msgrcv(redid, &p, sizeof(struct poruka), 2, 0);

        }while (strcmp(p.tekst, "KRAJ"));
        return 0;
    }
    else{
        //proc 1
        FILE* f = fopen("tekst.txt", "r");
        struct poruka p;

        int i = 1;
        while (!feof(f))
        {
            fgets(p.tekst,100, f);
            p.tip = i;
            msgsnd(redid, &p, sizeof(struct poruka), 0);
            puts(p.tekst);
            if(i == 2 ) i =1;
            else i =2;
        
        }
        fclose(f);

        strcpy(p.tekst ,"KRAJ");
        p.tip =1;
        msgsnd(redid, &p, sizeof(struct poruka), 0);
        p.tip =2;
        msgsnd(redid, &p, sizeof(struct poruka), 0);
        
        wait(NULL);
        wait(NULL);
        msgctl(redid, IPC_RMID, 0);
        semctl(procaid, 0, IPC_RMID, NULL);
		semctl(procbid, 0, IPC_RMID, NULL);
    }
    return 0;

}