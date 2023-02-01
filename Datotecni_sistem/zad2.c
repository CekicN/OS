/*Koriscenjem programskog jezika C napisati Linux program koji prikazuje statistiku za zadati direktorijum. Putanja do direktorijuma se zadaje kao argument komandne
linije.  */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>

int main(int argc, char* argv[])
{
    DIR* dp;
    struct dirent* dirp;
    int brfldr, brdat, brlnk;
    struct stat statbuf;
    int result;
    char path[255];

    if (argc!=2)
    {
        printf("Neophodno je 2 argumenta komandne linije.");
        exit(1);
    }

    if((dp=opendir(argv[1]))==NULL)
    {
        printf("Greska");
        exit(1);
    }

    brfldr=brdat=brlnk=0;

    while((dirp=readdir(dp))!=NULL)
    {
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        if((result=stat(path, &statbuf))==-1)
        {
            printf("Neuspesno");
            exit(1);
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            printf("Objekat %s je direktorijum\n", dirp->d_name);
            brfldr++;
        }

        if (S_ISREG(statbuf.st_mode))
        {
            printf("Objekat %s je datoteka\n", dirp->d_name);
            brdat++;
        }

        if (S_ISLNK(statbuf.st_mode))
        {
            printf("Objekat %s je link\n", dirp->d_name);
            brlnk++;
        }
    }
}