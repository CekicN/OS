/*Na programskom jeziku C napisati program koji kreira dodatni proces dete.Proces roditelj sa
standardnog ulaza prihvata podatke o studentima (broj indeksa i ime i prezime studenta)
i koriscenjem reda poruka te informacije salje procesu detetu. Proces dete ima zadata,da 
nakon sto primi podatke o poslednjem studentu,sortira studente po broju indeksa i tako 
dobijeni spisak odstampa na standardnom izlazu. */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

int brojStudenata;

struct student 
{
    int indeks;
    char ime[50];
    char prezime[50];
};

struct mymsgbuf
{
    long mtype;
    struct student student;
};

int main()
{
    int msqid;
    struct mymsgbuf buf;
    
    printf("Unesite broj studenata: ");
    scanf("%d",&brojStudenata);
    
    buf.mtype = 1;

    msqid = msgget(10104, 0666 | IPC_CREAT);
    
    if(fork() == 0)
    {
        //dete
        struct student studenti[brojStudenata];
        struct student pom;
        
        for(int i=0;i<brojStudenata;i++)
        {
            msgrcv(msqid, &studenti[i], sizeof(struct student), 0, 0);
        }    
        
        printf("\nDete iz redova poruka preuzelo: \n");
        for(int i=0;i<brojStudenata;i++)
            printf("%d %s %s\n", studenti[i].indeks, studenti[i].ime, studenti[i].prezime);
        printf("\n");
        
        for(int i=0;i<brojStudenata;i++)
            for(int j=1+i;j<brojStudenata;j++)
                if(studenti[i].indeks > studenti[j].indeks)
                {
                    pom = studenti[i];
                    studenti[i] = studenti[j];
                    studenti[j] = pom;
                }
                
        printf("Sortirana lista studenata: \n");
        for(int i=0;i<brojStudenata;i++)
            printf("%d %s %s\n", studenti[i].indeks, studenti[i].ime, studenti[i].prezime);
        printf("\n");
    }
    else
    {
        //roditelj
        struct student student;
        
        for(int i=0;i<brojStudenata;i++)
        {
            printf("\nIndeks: ");
            scanf("%d",&student.indeks);
            printf("Ime: ");
            scanf("%s",student.ime);
            printf("Prezime: ");
            scanf("%s",student.prezime);
            printf("Roditelj sa ulaza procitao: %d %s %s\n",student.indeks, student.ime, student.prezime);
            msgsnd(msqid, &student, sizeof(struct student), 0);
        }
        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);

    }
    
    return 0;
}