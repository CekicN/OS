#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
void nadji(char* arg, char* ime, long vel)
{
    DIR* dp = opendir(arg);
    struct dirent* dir;
    struct stat statbuf;
    char path[80];
    while((dir = readdir(dp)) != NULL)
    {
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        strcpy(path, arg);
        strcat(path, "/");
        strcat(path, dir->d_name);

        stat(path, &statbuf);


        if(S_ISREG(statbuf.st_mode))
        {
            if(strstr(dir->d_name, ime) != NULL && statbuf.st_size == vel)
            {
                printf("Datoteka: %s", ime);
            }
        }
        else if(S_ISDIR(statbuf.st_mode))
        {
            nadji(path, ime, vel);
        }
    }
    closedir(dp);
}
int main(int argc, char* argv[])
{
    DIR *ds;
    struct dirent* dir;
    struct stat statbuf;
    if(argc < 3)
    {
        printf("Mali broj argumenata");
        exit(1);
    }

    ds = opendir(argv[1]);
    if(ds == NULL)
    {
        printf("greska");
        exit(1);
    }
    char path[80];

    while((dir = readdir(ds)) != NULL)
    {
         if(strcmp(dir->d_name, ".")==0 || strcmp(dir->d_name, "..") == 0)
            continue;
        //printf("%s\n", dir->d_name);
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dir->d_name);
        stat(path, &statbuf);


        if(S_ISREG(statbuf.st_mode))
            nadji(argv[2], dir->d_name, statbuf.st_size);
    }
    closedir(ds);
    return 0;
}
