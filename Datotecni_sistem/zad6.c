/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu odredjuje naziv najvece datoteke. Ime direktorijuma se
prosledjuje programu kao argument komandne linije. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuf;
    char path[100];


    if (argc<2)
    {
        printf("argumenti");
        exit(1);
    }
    strcpy(path, argv[1]);

    if (stat(path, &statbuf)<0)
    {
        printf("greska");
        exit(1);
    }

    if (!S_ISDIR(statbuf.st_mode))
    {
        printf("Nije folder");
        exit(1);
    }
    
    dp=opendir(path);
    if (dp==NULL)
    {
        printf("greska");
        exit(1);
    }

    //dirp=readdir(dp);
    int size=0;
    char path1[100];
    char maxpath[100];
    while ((dirp=readdir(dp))!=NULL)
    {
        strcpy(path1, path);
        strcat(path1, "/");
        strcat(path1, dirp->d_name);

        stat(path1, &statbuf);

        if (S_ISREG(statbuf.st_mode))
        {
            if (statbuf.st_size>size)
            {
                size=statbuf.st_size;
                strcpy(maxpath, path1);
            }
        }

    }

    printf("Max datoteka je %s\n",maxpath);
    //fflush(stdout);
    closedir(dp);
}