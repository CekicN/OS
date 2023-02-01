/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu lista nazive svih datoteka koje u svom nazivu sadrze zadati string. 
Putanja do direktorijuma i string koji se trazi se zadaju kao argumenti komandne linije. */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>

int main(int argc, char* argv[])
{
    DIR* dp;
    struct dirent* dirp;
    if (argc!=3)
    {
        printf("Nema dovoljno argumenata komandne linije");
        exit(1);
    }
    if((dp=opendir(argv[1]))==NULL)
    {
        printf("Greska");
        exit(1);
    }

    while ((dirp=readdir(dp))!=NULL)
    {
        if (strstr(dirp->d_name, argv[2])!=NULL)
            printf("%s\n", dirp->d_name);
    }
    closedir(dp);

}