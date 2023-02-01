#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>

int d = 0;
void obrisi(char *ime)
{
    DIR* dp = opendir(ime);
    struct dirent *dir;
    struct stat statbuf;

    char path[100];
    while((dir = readdir(dp)) != NULL && d < 3)
    {
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        strcpy(path, ime);
        strcat(path, "/");
        strcat(path, dir->d_name);

        stat(path, &statbuf);


        if(S_ISREG(statbuf.st_mode) && statbuf.st_size > 10 * 1024)
        {
            printf("%s\n", path);
            if(fork() == 0)
                execlp("rm", "rm", path, NULL);
            else
                wait(NULL);
        }
        else if(S_ISDIR(statbuf.st_mode))
        {
            d++;
            obrisi(path);
        }
    }
    closedir(dp);
}
int main(int argc, char* argv[])
{
    obrisi(argv[1]);
    return 0;
}
