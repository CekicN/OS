/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu (zadaje se
kao ulazni argument programa) odredjuje i brise pet najvecih regularnih datoteka.Za brisanje
regularnih datoteka koristiti Linux komandu rm. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

struct datoteka {
    char nazivDatoteke[1024];
    int velicinaDatoteke;
};

struct datoteka mojaDatoteka[50];
int brojDatoteka = 0;


int main(int argc, char* argv[]) {
  
    DIR* myDir;
    struct dirent * dirp;
    struct stat statBuffer;

    char dirName[1000], path[1000];

    if(argc != 2) {
        printf("Nevalidan broj ulaznih argumenata linux konzole\n");
        exit(-1);
    } else {
        printf("Radi\n");
    }

    strcpy(dirName, argv[1]);

    myDir = opendir(dirName);

    if(myDir == NULL) {
        printf("Lose otvoren direktorijum\n");
        exit(-1);
    } else {
        printf("%s otvoren\n\n", dirName);
    }

    while ((dirp = readdir(myDir)) != NULL)
    {   
       
        strcpy(path, dirName);
        strcat(path, "/");
        strcat(path, dirp->d_name);
        stat(path, &statBuffer);
        if (S_ISREG(statBuffer.st_mode)){
           
            strcpy(mojaDatoteka[brojDatoteka].nazivDatoteke, path);
            mojaDatoteka[brojDatoteka].velicinaDatoteke = statBuffer.st_size;
            brojDatoteka++;
        }
    
    }
    for(int i=0;i<brojDatoteka-1;i++) {
        for(int j=i+1;j<brojDatoteka;j++) {
                if(mojaDatoteka[i].velicinaDatoteke < mojaDatoteka[j].velicinaDatoteke) {
                    struct datoteka pomocna = mojaDatoteka[i];
                    mojaDatoteka[i] = mojaDatoteka[j];
                    mojaDatoteka[j] = pomocna;
                
            }
        }
    }

    for(int i=0;i<5;i++) {
        stat(mojaDatoteka[i].nazivDatoteke, &statBuffer);
        int pid1 = fork();
        if(pid1 == 0) {
            execlp("rm", "rm", mojaDatoteka[i].nazivDatoteke, NULL);
            brojDatoteka--;
        } else {
            wait(NULL);
        }
    }

    printf("\nNAKON BRISANJA\n\n");
    for(int i=0;i<brojDatoteka;i++) {
         printf("%d ^^ %s ^^ %d \n", i, mojaDatoteka[i].nazivDatoteke,
         mojaDatoteka[i].velicinaDatoteke);
    }
    return 0;
}