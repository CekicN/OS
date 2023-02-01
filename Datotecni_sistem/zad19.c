/*
Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu odredjuje
i stampa naziv najmanje datoteke sa ekstenzijom .txr i najvece datoteke sa ekstenzijom .dat.
Ime direktorijuma koji se pretrazuje se prosledjuje programu kao argument komandne linije.
*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_NAME 1024

int main(int argc, char * argv[])
{

	char dirname[MAX_NAME];
	char path[MAX_NAME];
	struct stat statbuf;
	DIR * dp;
	struct dirent * dirp;

	if (argc < 2)
	{
		printf("Nema dovoljno ulaznih argumenata\n");
		exit(-1);
	}

	strcpy(dirname, argv[1]);
	strcpy(path, argv[1]);

	//proveravamo da li je uneta putanja validna
	if (stat(dirname, &statbuf) < 0)
	{
		printf("Doslo je do greske prilikom ocitavanja statusa unete putanje\n");
		exit(-1);
	}

	//da li je uneta putanja do direktorijuma
	if (!S_ISDIR(statbuf.st_mode))
	{
		printf("Uneti argument ne predstavlja putanu do direktorijuma\n");
		exit(-1);
	}
    dp = opendir(path);
    if (dp == NULL)
    {
        printf("Greska prilikom otvaranja direktorijuma\n");
        exit(-1);
    }

    int minVel = 10000000;
    int maxVel = 0;
    char min[MAX_NAME] = " ";
    char max[MAX_NAME] = " ";

    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
			continue;

        strcpy(path, dirname);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statbuf);
        
        printf("Obradjujem: %s\n", path);
        printf("%ld\n", statbuf.st_size);
        printf("%d\n", S_ISREG(statbuf.st_mode));
        if (S_ISREG(statbuf.st_mode) && strstr(path, ".txt") != NULL && minVel > statbuf.st_size){          
                strcpy(min, path);
                minVel = statbuf.st_size;
                printf("PROMENA MIN ---  %s --- %d\n", min, minVel);
        }else if(S_ISREG(statbuf.st_mode) && strstr(path, ".bin") != NULL && maxVel < statbuf.st_size){
                strcpy(max, path);
                maxVel = statbuf.st_size;
                printf("PROMENA MAX ---  %s --- %d\n", max, maxVel);

        }
    }
    closedir(dp);
    printf("%s, %d\n", min, minVel);
    printf("%s, %d \n", max, maxVel);

    return 0;
}