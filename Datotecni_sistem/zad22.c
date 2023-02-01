/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji za zadati direktorijum štampa
imena svih regularnih datoteka koje se u njemu nalaze sortirane prema veličini datoteke. Ime
direktorijuma koji se obrađuje i smer sortiranja (opadajudi ili rastudi) se prosleđuju programu kao
argumenti komandne linije. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define MAX_FILES 200

struct dat {
    char name[50];
    int size;
};

int veci(struct dat prvi,struct dat drugi)
{
    if(prvi.size > drugi.size)
        return 1;
    else
        return 0;
}

int main(int argc,char* argv[])
{
    if(argc != 3){printf("argumenti!");return 0;}
    struct dat files[MAX_FILES]; 
   struct dat pom;
    int opadajuce = atoi(argv[2]);
    char path[250];
    struct dirent *direntbuf;
    struct stat statbuf;
    int i =0,j,n;
   // if(opadajuce != 0 || opadajuce != 1) {printf("smer sortiranja!");return 0;}
    DIR* dir = opendir(argv[1]);
    
    while((direntbuf = readdir(dir)) != NULL)
    {
        if(strcmp(direntbuf->d_name,".") == 0 || strcmp(direntbuf->d_name,"..") == 0) continue;
        
        strcpy(path,argv[1]);
        strcat(path,"/");
        strcat(path,direntbuf->d_name);
        
        stat(path,&statbuf);
        
        if(S_ISREG(statbuf.st_mode))
        {
            strcpy(files[i].name,direntbuf->d_name);
            files[i].size = statbuf.st_size;
            i++;
        }
        
    }
    closedir(dir);
    
    n = i;
    for(i=0;i<n;i++)
        for(j=i;j<n;j++)
        {
            if(veci(files[i],files[j]) == opadajuce)
            {
                pom = files[i];
                files[i] = files[j];
                files[j] = pom;
            }
        }
    
    
    for(i = 0;i <n;i++)
        printf("%s\t%d\n",files[i].name,files[i].size);
    
}