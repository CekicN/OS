/*Korišćenjem programskog jezika C napisati Linux program koji u zadatom
direktorijumu (putanja do direktorijuma se zadaje kao ulazni argument programa)
rekurzivno nalazi pet najvećih regularnih datoteka. 
Program na kraju rada treba da odštampa putanje i veličine tih datoteka.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_NAME 1024
#define false 0
#define true 1

int nizVelicina [5] = {0,0,0,0,0};
char nizPutanja[5][1024] = {" 1", " 2", " 3", " 4", " 5"};

void processdir(char* foldername){
    DIR* dp;
    struct dirent* dirp;
    struct stat statbuf;
    int result;
    if ((dp = opendir(foldername)) == NULL)
    {
        printf("Greska prilikom otvaranja foldera %s!\n", foldername);
        return;
    }
    while( (dirp = readdir(dp)) != NULL )
    {
        char tmp[1024] = "";
        strcat(tmp, foldername);
        strcat(tmp, "/");
        strcat(tmp, dirp->d_name);
        //printf("Obradjujem %s u %s.\n", dirp->d_name, tmp);
        if ((result = stat(tmp, &statbuf)) == -1)
        {
            printf("Neuspesno citanje podataka o objektu: %s\n", tmp);
            continue;
        }
        if(S_ISREG(statbuf.st_mode))
        {
            int i =4;
            int promena = false;
            while (i>=0 && !promena)
            {
                if(nizVelicina[i]<statbuf.st_size){
                    
                    //ide promena
                    
                    for(int j = 0; j<=i;j++){
                        nizVelicina[j] = nizVelicina[j+1];
                        strcpy(nizPutanja[j], nizPutanja[j+1]);
                    }
                    nizVelicina[i] = statbuf.st_size;
                    strcpy(nizPutanja[i], tmp);
                    promena = true;
                }
                i--;
            }
        }
        else if (S_ISDIR(statbuf.st_mode)
            && strcmp(dirp->d_name, ".") != 0
            && strcmp(dirp->d_name, "..") != 0)
        {
            printf("Otvaram folder %s.\n", tmp);
            processdir(tmp);
        }
    }
    closedir(dp);
}

int main(int argc, char * argv[])
{
    
    if (argc < 2)
    {
        printf("Nema dovoljno ulaznih argumenata\n");
        exit(-1);
    }
    puts(argv[1]);
    processdir(argv[1]);
    
    for(int i=4; i>=0;i--){
        printf("%d. fajl: \n", i+1);
        puts(nizPutanja[i]);
        printf("%d \n", nizVelicina[i]);
    }

}