/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu rekurzivno, maksimalno do dubine 3, brise sve regularne datoteke
velicine vece od 10KB. Za brisanje datoteka koristiti rm komandu Linux operativnog sistema. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_NAME 1024
void processdir(char * path, int level);

int main(int argc, char* argv[])
{
    char dirname[MAX_NAME];
    char path[MAX_NAME];
    struct stat statbuf;
    DIR* dp;
    struct dirent *dirp;
    int count, size;

    if (argc<2)
    {
        printf("Nedovoljno argumenata");
        exit(1);
    }

    strcpy(dirname, argv[1]);

    if(stat(dirname, &statbuf)<0)
    {
        printf("Greska");
        exit(1);
    }

    if (!S_ISDIR(statbuf.st_mode))
    {
        printf("Greska");
        exit(1);
    }

    processdir(dirname, 1);
}

void processdir(char* path, int level)
{
    DIR* dp;
    struct dirent* dirp;
    char path1[MAX_NAME];
    struct stat statbuf;
    dp=opendir(path);
    if (dp==NULL)
    {
        printf("Greska");
        exit(1);
    }

    while((dirp=readdir(dp))!=NULL)
    {
        if (strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0)
            continue;
        strcpy(path1, path);
        strcat(path1, "/");
        strcat(path1, dirp->d_name);

        stat(path1, &statbuf);
        if (S_ISREG(statbuf.st_mode)&&statbuf.st_size>10*1024)
        {
            if (fork()==0)
            {
                execlp("rm", "rm", path1, NULL);
            }
            else
            {
                wait(NULL);
            }
        }
        else if(S_ISDIR(statbuf.st_mode)&&level<3)
            processdir(path1, level+1);
    }
    closedir(dp);
}