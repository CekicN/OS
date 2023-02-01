#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>

void nadji(char* arg, char* ime)
{
    DIR * dp = opendir(arg);
    if(dp == NULL)
        exit(1);
    struct dirent* dir;
    struct stat statbuf;
    char path[100];
    while((dir = readdir(dp)) != NULL)
    {
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        strcpy(path, arg);
        strcat(path, "/");
        strcat(path, dir->d_name);
        stat(path, &statbuf);

        if(S_ISREG(statbuf.st_mode) && strcmp(dir->d_name, ime) == 0)
        {
            printf("Putanja je: %s\nVelicina je: %ld", path, statbuf.st_size);
        }
        else if(S_ISDIR(statbuf.st_mode))
        {
            nadji(path,ime);
        }
    }
    closedir(dp);
}
int main(int argc, char* argv[])
{
    nadji(argv[1], argv[2]);
    return 0;
}
