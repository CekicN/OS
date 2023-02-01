#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<sys/wait.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<sys/wait.h>

struct student
{
    char ime[50];
    char prezime[50];
    int indeks;
};
struct poruka
{
    long type;
    struct student stud;
};

void printaj(struct student p)
{
    printf("Ime: %s, Prezime: %s, Indeks: %d\n", p.ime, p.prezime, p.indeks);
}
int main()
{
    int msqid;
    msqid = msgget(10000, 0666|IPC_CREAT);
    struct poruka p;
    p.type = 1;
    if(fork() != 0)
    {
        //roditelj
        struct student student;
        int n;
        printf("Koliko studenata:");
        scanf("%d", &n);
        int i;

        for(i = 0; i< n;i++)
        {

            printf("Unesite ime:");
            scanf("%s", &student.ime);
            printf("Unesite prezime:");
            scanf("%s", &student.prezime);
            printf("Unesite indeks:");
            scanf("%d", &student.indeks);

            p.stud = student;
            msgsnd(msqid, &p, sizeof(p.stud), 0);
        }

        strcpy(student.ime, "kraj");
        strcpy(student.prezime, "kraj");
        student.indeks = -1;
        p.stud = student;

        msgsnd(msqid, &p, sizeof(p.stud), 0);
        wait(NULL);
        msgctl(msqid, IPC_RMID, 0);
    }
    else
    {
        //dete
        struct student studenti[50];
        int i = 0;
        do
        {
            msgrcv(msqid, &p, 1024, 0, 0);
            studenti[i++] = p.stud;
        }while(strcmp(p.stud.ime, "kraj") != 0 && strcmp(p.stud.prezime, "kraj") != 0 && p.stud.indeks != -1);
        i--;
        int j = 0;
        int k;
        for(;j < i-1; j++)
        {
            for(k = j+1; k < i; k++)
            {
                if(studenti[j].indeks > studenti[k].indeks)
                {
                    struct student pom = studenti[j];
                    studenti[j] = studenti[k];
                    studenti[k] = pom;
                }
            }
        }
        for(j = 0; j < i; j++)
            printaj(studenti[j]);
    }
}
