#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
int naso = 0;
void nadji(char* arg, char* ime, long vel, char* p, char *z)
{
    DIR* dp = opendir(arg);
    struct dirent* dir;
    struct stat statbuf;
    char path[80];
    while((dir = readdir(dp)) != NULL && naso == 0)
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
                naso = 1;
                printf("Datoteka: %s", ime);
            }
        }
        else if(S_ISDIR(statbuf.st_mode))
        {
            nadji(path, ime, vel, p,z);
        }
    }
    closedir(dp);

    if(naso == 0)
    {
        printf("%s --- %s\n", p, arg);
        naso = 1;
        if(fork() == 0)
        {
            execlp("cp", "cp", p,z, NULL);
        }
        else
            wait(NULL);
    }
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

        naso = 0;
        if(S_ISREG(statbuf.st_mode))
            nadji(argv[2], dir->d_name, statbuf.st_size, path, argv[2]);
    }
    closedir(ds);
    return 0;
}
