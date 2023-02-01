/*Koriscenjem programskog jezika C napisati Linux program koji kao argument komandne linije prihvata jedan string i putanju do direktorijuma. 
Program u tom direktorijumu i svim njegovim poddirektorijumima maksimalno do dubine 10 pronalazi sve regularne datoteke cija imena pocinju stringom koji je prvi 
argument komandne linije i na ekranu stampa njihova imena. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define PATH 200

void processdir(char dir[], char str[], int level)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuf;
    char path[PATH]="";

    if ((dp=opendir(dir))==NULL)
    {
        printf("Greska1");
        exit(1);
    }

    int i=strlen(str);

    while((dirp=readdir(dp))!=NULL)
    {
        if (strcmp(path, ".")==0 || strcmp(path, "..")==0)
            continue;
        strcpy(path, dir);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statbuf);

        if (S_ISREG(statbuf.st_mode))
        {
            char p[PATH];
            strncpy(p, dirp->d_name, i);
            if (strcmp(p, str)==0)
            {
                printf("Pronadjena datoteka %s\n", dirp->d_name);
            }
        }
        else if (S_ISDIR(statbuf.st_mode))
        {
            if (level<10)
                processdir(path, str, level+1);
        }

    }
    closedir(dp);

}

int main(int argc, char* argv[])
{
    char string[PATH];
    char put[PATH];
    struct stat statbuf2;

    if (argc<3)
    {
        printf("Nedovoljno argumenata");
        exit(1);
    }

    strcpy(string, argv[1]);
    strcpy(put, argv[2]);

    if (stat(put,&statbuf2 )<0)
    {
        printf("greska2");
        exit(1);
    }

    if (!S_ISDIR(statbuf2.st_mode))
    {
        printf("Greska3");
        exit(1);
    }

    processdir(put, string, 1);


}