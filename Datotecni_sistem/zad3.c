/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu pronalazi i stampa nazive svih datoteka koje u imenu sadrze string koji 
se prosledjuje kao drugi argument komandne linije. Pretrazivanje se obavlja rekurzivno u zadatom direktorijumu i svim njegovim poddirektorijumima */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>

void processdir(char* foldername, char* str)
{
    DIR* dp;
    struct dirent* dirp;
    int brfldr, brdat, brlnk;
    struct stat statbuf;
    int result;

    if ((dp=opendir(foldername))==NULL)
    {
        printf("Greska");
        exit(1);
    }

    while((dirp=readdir(dp))!=NULL)
    {
        char tmp[1024]="";
        strcat(tmp, foldername);
        strcat(tmp, "/");
        strcat(tmp, dirp->d_name);

        printf("Obradjujem %s u %s\n", dirp->d_name, tmp);

        if ((result=stat(tmp, &statbuf))==-1)
        {
            printf("greska");
            continue;

        }
        if (strstr(dirp->d_name, str)!=NULL)
        printf("Pronasao %s\n", tmp);

        if(S_ISDIR(statbuf.st_mode)&&strcmp(dirp->d_name, ".")!=0 && strcmp(dirp->d_name, "..")!=0)
        {
            printf("Otvaram folder %s", tmp);
            processdir(tmp, str);
        }
        
    }
    closedir(dp);
}

int main(int argc, char* argv[])
{
    processdir(argv[1], argv[2]);
}