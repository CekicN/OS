/*Koriscenjem programskog jezika C napisati Linux program koji sinhronizuje sadrzaj
dva direktorijuma. Putanje do direktorijuma koji se sinhronizuju se zadaju kao
ulazni argumenti programa. Program za svaku regularnu datoteka iz prvog direktorijuma
proverava da li postoji odgovarajuca datoteka u drugom direktorijuma (uporeduje ime 
i velicinu datoteke). Ukoliko datoteka ne postoji, onda se ona kopira iz prvog u drugi direktorijum. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc,char* argv[])
{
    if(argc != 3) return 0;
    
    DIR* prviDir = opendir(argv[1]);
    DIR* drugiDir = opendir(argv[2]);
    if(prviDir == NULL || drugiDir == NULL)
    {
        printf("Neki od direktorijuma se nije otvorio!\n");
        return 0;
    }
    char prviPath[1024];
    char drugiPath[1024];
    struct dirent *prviDirent;
    struct dirent *drugiDirent;
    struct stat prviStat;
    struct stat drugiStat;
    int flag = 0;
    
    while((prviDirent = readdir(prviDir)) != NULL)
    {
        if(strcmp(prviDirent->d_name,".") == 0 || strcmp(prviDirent->d_name,"..") == 0)
            continue;
        strcpy(prviPath,argv[1]);
        strcat(prviPath,"/");
        strcat(prviPath,prviDirent->d_name);
        stat(prviPath,&prviStat);
        printf("Obradjujem prvi: %s\n",prviPath);
        if(S_ISREG(prviStat.st_mode))
        {
            flag = 0;
            rewinddir(drugiDir);
            while((drugiDirent = readdir(drugiDir)) != NULL && flag == 0)
            {
                if(strcmp(drugiDirent->d_name,".") == 0 || strcmp(drugiDirent->d_name,"..") == 0)
                    continue;
                strcpy(drugiPath,argv[2]);
                strcat(drugiPath,"/");
                strcat(drugiPath,drugiDirent->d_name);
                stat(drugiPath,&drugiStat);
                
                printf("Obradjujem drugi: %s\n",drugiPath);
                if(S_ISREG(drugiStat.st_mode) && strcmp(prviDirent->d_name,drugiDirent->d_name) == 0 && prviStat.st_size == drugiStat.st_size)
                    printf("Postavljam flag na %d\n",flag = 1);
            }
            printf("Flag je %d\n",flag);
            if(flag == 0)
            {
                //kopiranje
                if(fork() == 0)
                    execlp("cp","cp",prviPath,argv[2],NULL);
                else
                    wait(NULL);
            }
        }
        
        printf("\n");
    }
    
    closedir(prviDir);
    closedir(drugiDir);
    
}