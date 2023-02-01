/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji u zadatom direktorijumu
direktorijumu i njegovim poddirektorijumima nalazi sve datoteke koje imaju ekstenziju .dat i vede su
od neke zadate granice Ime direktorijuma koji se pretražuje i granica se prosleđuju programu kao
argumenti komandne linije. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

void citajDir(char* path,int granica);

int main(int argc,char* argv[])
{
    if(argc != 3) { printf("argumenti!\n"); return 0;}
    
    citajDir(argv[1],atoi(argv[2]));
    
}

void citajDir(char* path,int granica)
{
    char npath[512];
    struct dirent *direntbuf;
    struct stat statbuf;
    DIR* dir = opendir(path);
    if(dir == NULL) { printf("direktorijum!\n"); return;}
    
    while((direntbuf = readdir(dir)) != NULL)
    {
        if(strcmp(direntbuf->d_name,".") == 0 || strcmp(direntbuf->d_name,"..") == 0)
            continue;
        
        strcpy(npath,path);
        strcat(npath,"/");
        strcat(npath,direntbuf->d_name);
        
        stat(npath,&statbuf);
        
        if(S_ISDIR(statbuf.st_mode) )
            citajDir(npath,granica);
        else if(S_ISREG(statbuf.st_mode))
        {
            if(statbuf.st_size > granica && strstr(direntbuf->d_name,".dat") != NULL) 
                printf("%s\t%d\n",npath,statbuf.st_size);
        }
    }
    
    closedir(dir);
}