/*
Koriscenjem programskog jezika C napisati Linux program koji 
u zadatom direktorijumu (kao ulazni argument programa zadaje se
apsolutna putanja do direktorijuma) i njevoim poddirektorijumima
odredjuje i imena svih regularnih datoteka koje su vece od 100KB
i tako dobijeni spisak,sortiran po velicini datoteke u rastucem
redosledu,stampa na standardni izlaz.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_NAME 1024

void processdir(char * path);

struct datoteka
{
    char imeDatoteke[MAX_NAME];
    int velicinaDatoteke;
};

struct datoteka datoteke[100];
int broj = 0;

int main(int argc, char * argv[])
{
    char dirname[MAX_NAME];
    char path[MAX_NAME];
    struct stat statbuf;
    DIR * dp;
    struct dirent * dirp;
    int count, size;
    if (argc < 2)
    {
        printf("Nema dovoljno ulaznih argumenata\n");
        exit(-1);
    }
    strcpy(dirname, argv[1]);

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

    processdir(dirname);

    for(int i =0; i<broj;i++){
        for(int j = i+1; j<broj;j++){
            if(datoteke[i].velicinaDatoteke>datoteke[j].velicinaDatoteke){
                struct datoteka pom = datoteke[i];
                datoteke[i] = datoteke[j];
                datoteke[j] = pom;                       
            }
        }
    }

    for(int i =0; i<broj;i++){
        printf("Ime:%s, velicina:%d\n", datoteke[i].imeDatoteke, datoteke[i].velicinaDatoteke);
    }
    return 0;
}

void processdir(char * path)
{
    DIR * dp;
    struct dirent * dirp;
    char path1[MAX_NAME];
    struct stat statbuf;
    dp = opendir(path);
    if (dp == NULL)
    {
        printf("Greska prilikom otvaranja direktorijuma\n");
        exit(-1);
    }
    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0
        || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path1, path);
        strcat(path1, "/");
        strcat(path1, dirp->d_name);
        stat(path1, &statbuf);
        
        if (S_ISREG(statbuf.st_mode) && statbuf.st_size > 1 *1024)
        {
           strcpy(datoteke[broj].imeDatoteke, dirp->d_name);
           datoteke[broj].velicinaDatoteke = statbuf.st_size;
           broj++;
        }
        else if (S_ISDIR(statbuf.st_mode))
        {
            processdir(path1);
        }
    }
    closedir(dp);
}