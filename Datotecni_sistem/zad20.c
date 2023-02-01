/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji za unete donju i gornju granicu
veličine (u bajtovima) sa tastature u zadatom direktorijumu određuje i štampa naziv najmanje
datoteke sa ekstenzijom .c a čija veličina se nalazi u unetom opsegu. Ime direktorijuma koji se
pretražuje se prosleđuje programu kao argument komandne linije.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc,char* argv[])
{
    if(argc != 4) {printf("argumenti");return 0;}
    
    struct dirent  *direntbuf;
    struct stat statbuf;
    
    DIR* dir = opendir(argv[1]);
    int dg = atoi(argv[2]);
    int gg = atoi(argv[3]);
    char path[256];
    
    char min[100];
    int minSize = -1;
    
    while((direntbuf = readdir(dir)) != NULL)
    {
        strcpy(path,argv[1]);
        strcat(path,"/");
        strcat(path,direntbuf->d_name);
        stat(path,&statbuf);
        
        if(S_ISREG(statbuf.st_mode) && statbuf.st_size > dg && statbuf.st_size < gg && strstr(direntbuf->d_name,".c") != NULL)
        {
            if(minSize == -1 || minSize > statbuf.st_size)
            {
                minSize = statbuf.st_size;
                strcpy(min,direntbuf->d_name);
            }
        }
    }
    
    printf("Najmanji dokument %s\t%d\n",min,minSize);
    
    closedir(dir);
    
}


