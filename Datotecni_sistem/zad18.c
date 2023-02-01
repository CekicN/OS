/*Koriscenjem prog jez C napisati Linux prog koji pocesvsi od korenskog (root) direktorijuma, pronalazi
sve reg datoteke cija je apsolutna putanja duza od 256 karaktera i cija je velicina manja od 2KB. */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void obradiDirektorijum(char* path);

int main()
{
    char path[50];
    strcpy(path,"/home");
    obradiDirektorijum(path);
    return 0;
}


void obradiDirektorijum(char* path)
{
    DIR* dir = opendir(path);
    if(dir == NULL){/* printf("direktorijum\t%s\n",path);*/return; }
    struct dirent *direntbuf;
    struct stat statbuf;
    char npath[2048];
    
    while((direntbuf = readdir(dir)) != NULL)
    {
        if(strcmp(direntbuf->d_name,".") == 0 || strcmp(direntbuf->d_name,"..") == 0)
            continue;
        strcpy(npath,path);
        if(strcmp(npath,"/") != 0)
            strcat(npath,"/");
        strcat(npath,direntbuf->d_name);
        
        stat(npath,&statbuf);

        if (S_ISREG(statbuf.st_mode)) {
            if (statbuf.st_size < (2048) && strlen(npath) > 128) {
                printf("%s : %.2f KB \n", npath, (statbuf.st_size) / 1024.0);
            }
        } else if(S_ISDIR(statbuf.st_mode)) {
            obradiDirektorijum(npath);
        }
    }
}