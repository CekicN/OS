/*Korišćenjem programskog jezika C napisati Linux/UNIX program koji u zadatom direktorijumu i njegovim poddirektorijumima 
nalazi sve datoteke koje imaju ekstenziju .sh i pokušava da ih izvrši (korišćenjem komande sh). 
Putanja do direktorijuma koji se pretražuje (apsolutna ili relativna) se prosleđuju programu kao argument komandne linije*/

/*PRVO IH LEPO ODRADI KAO PREKO OVOG PRINTF ALI ONDA DALJE NASTAVLJA, POGLEDAJ KAKO TO DA SREDIMO*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>

#define MAX_NAME 1024
int i=1;

void processdir(char *path, int level)
{
    DIR *dp;
    struct dirent *dirp;
    struct stat statbuf;
    char path1[MAX_NAME];

    if ((dp = opendir(path)) == NULL)
    {
        printf("Greska prilikom otvaranja direktorijuma!\n");
        exit(-1);
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path1,"");
        strcat(path1, path);
        strcat(path1, "/");
        strcat(path1, dirp->d_name);

        if (stat(path1, &statbuf) < 0)
        {
            printf("Nije moguce procitati podatke o stavci.\n");
            continue;
        }

        if (S_ISREG(statbuf.st_mode) && strstr(dirp->d_name, ".c") != NULL)
        {

            printf("Pronadjena datoteka: %s\n",dirp->d_name);
            int pid;
            pid = fork();

            if (pid == 0)
            {
                //execl(path1,path1,NULL);//Proveri da li se tako izvrsavaju
                printf("Pozdrav iz deteta koji obavlja skriptu %d.\n",i++);
                fflush(stdout);
            }
            else
            {
                wait(NULL);
            }
        }
        else
        {
            if (S_ISDIR(statbuf.st_mode))
                processdir(path1, level + 1);
        }
    }
    closedir(dp);
}

int main(int argc, char *argv[])
{
    char dirname[MAX_NAME];
    struct stat statbuf;
    int result;

    if (argc < 2)
    {
        printf("Neophodno je zadatu putanju do direktorijuma!\n");
        exit(-1);
    }

    strcpy(dirname, argv[1]);

    result = stat(dirname, &statbuf);

    if (result == -1)
    {
        printf("Nije moguce procitati informacije o zadatoj stavci!\n");
        exit(-1);
    }

    if (!S_ISDIR(statbuf.st_mode))
    {
        printf("Zadata putanja nije putanja do direktorijuma!\n");
        exit(-1);
    }

    processdir(dirname, 1);

    return 0;
}