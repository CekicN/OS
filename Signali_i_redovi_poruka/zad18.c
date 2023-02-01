/*
Korišćenjem programskog jezika C napisati Linux/UNIX program koji kreira dodatni process sa kojim komunicira dvosmerno korišćenjem jednog reda poruka. 
Proces roditelj iz datoteke čije je ime prosleđeno kao argument komandne linije čita liniju po liniju teksta i svaku liniju šalje procesu detetu u posebnoj poruci.
Proces dete u svakoj primljenoj liniji prebrojava broj pojavljivanja velikih slova abecede i procesu roditelju vraća kroz isti red poruka za svaku analiziranu 
liniju teksta kroz posebnu poruku redni broj linije i broj pojavljivanja velikih slova. Glavni program te podatke štampa na ekranu 
(redni broj linije i broj pojavljivanja velikih slova). Kada se analiziraju sve linije teksta iz datoteke glavni program na ekranu treba da odštampa “KRAJ”. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define RED_PORUKA 10011
#define MAX_PORUKA 50
int rbPoruke=0;

struct poruka
{
    long tip;
    char tekst[MAX_PORUKA];
};

int main(int argc,char* argv[])
{
    int pid;
    int redid;
    struct poruka bafer;
    FILE* f;
    char pom[MAX_PORUKA];


    redid=msgget(RED_PORUKA,IPC_CREAT | 0666);


    pid=fork();

    if(pid<0)
    {
        printf("Greska prilikom forkovanja!");
        exit(1);
    }

    if(pid != 0)
    {
        f=fopen(argv[1],"r");
        fgets(pom,MAX_PORUKA,f);
        do
        {
            strcpy(bafer.tekst,pom);
            bafer.tip=1;
            if(msgsnd(redid,&bafer,MAX_PORUKA,0)<0)
            {
                 printf("Greska prilikom slanja poruke!");
                 exit(0);
            }

            if(msgrcv(redid,&bafer,MAX_PORUKA,2,0)<0)
            {
                printf("Greska prilikom citanja!");
                exit(0);
            }

            printf("Redni broj i broj velikih slova: %s\n",bafer.tekst);
            //poruka++;
            fgets(pom,MAX_PORUKA,f);
        }
        while(!feof(f));

        bafer.tip=1;
        strcpy(bafer.tekst,"KRAJ");
        if(msgsnd(redid,&bafer,MAX_PORUKA,0)<0)
        {
            printf("Greska prilikom slanja poruke!");
            exit(0);
        }

        printf("KRAJ\n");

        wait(NULL);
        msgctl(redid,IPC_RMID,0);
        return 0;

    }
    else
    {
        char pom2[MAX_PORUKA];
        do{
            if(msgrcv(redid,&bafer,MAX_PORUKA,1,0) < 0)
            {
                printf("Greska prilikom citanja!");
                exit(0);
            }


            int i;
            strcpy(pom2,bafer.tekst);
            if(strcmp(pom2,"KRAJ")!=0)
            {
                int k=0;
                for(i=0;i<strlen(pom2);i++)
                {
                    if(pom2[i]>='A' && pom2[i]<='Z')
                        k++;
                }

                rbPoruke++;
                bafer.tip=2;
                char pom3[MAX_PORUKA]="";
                char pom4[MAX_PORUKA]="";
                sprintf(pom3,"%d ",rbPoruke);
                sprintf(pom4,"%d ",k);
                strcat(pom3,pom4);
                strcpy(bafer.tekst,pom3);
                if(msgsnd(redid,&bafer,MAX_PORUKA,0)<0)
                {
                    printf("Greksa prilikom slanja!");
                    exit(0);
                }
            }
        }
        while(strcmp(pom2,"KRAJ")!=0);

        exit(0);
    }
}