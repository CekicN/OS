/*Koriscenjem programskog jezika C napisati Linux program koji rekurzivno pretrazuje stablo zadatog direktorijuma u potrazi za zadatom datotekom ili direktorijumom. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define PATH 200

void processdir(char dir[], char dat[])
{
    printf("%s\n", dir);
        DIR* dp;
    struct dirent* dirp;
    struct stat statbuf2;
    char path[PATH];
    

    if ((dp=opendir(dir))==NULL)
    {
        printf("greska3");
        exit(1);
    }

    while((dirp=readdir(dp))!=NULL)
    {
        if (strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0)
            continue;
        strcpy(path, dir);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statbuf2);
        if (S_ISREG(statbuf2.st_mode) && strcmp(dirp->d_name, dat)==0)
        {
            printf ("Datoteka %s je pronadjena\n", dat);
        }
        else
        {
            if (S_ISDIR(statbuf2.st_mode))
            {
                if (strcmp(dirp->d_name, dat)==0)
                printf ("Direktorijum %s je pronadjen\n", dat);
                else
            {
                processdir(path, dat);
            }
            }
            
        }
    }

    closedir(dp);
}

int main(int argc, char* argv[])
{
    char dir[PATH];
    char dat[PATH];
    struct stat statbuf;
    if (argc<3)
    {
        printf("greska1");
        exit(1);
    }

    strcpy(dir, argv[1]);
    strcpy(dat, argv[2]);

    printf("%s\n", dat);

    if (stat(dir,&statbuf )<0)
    {
        printf("greska2");
        exit(1);
    }

    if (!S_ISDIR(statbuf.st_mode))
    {
        printf("Greska");
        exit(1);
    }

    processdir(dir, dat);
}