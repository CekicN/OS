#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
struct path_size
{
    char path[100];
    long size;
};
void print(struct path_size ps)
{
    printf("PATH: %s, SIZE:%ld\n", ps.path, ps.size);
}

struct path_size ps[50];
void obidji(char* ime)
{
    DIR* dp = opendir(ime);
    struct dirent* dir;
    struct stat statbuf;

    struct path_size s;
    char path[100];
    int n = 0;
    while((dir = readdir(dp)) != NULL)
    {
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        strcpy(path, ime);
        strcat(path, "/");
        strcat(path, dir->d_name);
        stat(path, &statbuf);
        if(S_ISREG(statbuf.st_mode))
        {
            strcpy(s.path,path);
            s.size = statbuf.st_size;
            ps[n++] = s;
        }
        else if(S_ISDIR(statbuf.st_mode))
            obidji(path);

    }
    int i;
    int j;
    for(i = 0; i < n-1; i++)
    {
        for(j = i+1; j < n; j++)
        {
            if(ps[i].size < ps[j].size)
            {
                s = ps[i];
                ps[i] = ps[j];
                ps[j] = s;
            }
        }
    }
    closedir(dp);
}
int main(int argc, char* argv[])
{
    obidji(argv[1]);
    int i;
    for(i = 0; i < 5;i ++)
    {
        printf("BRISI...\n");
        print(ps[i]);
        if(fork() == 0)
            execlp("rm", "rm", ps[i].path, NULL);
        else
            wait(NULL);
    }
    return 0;
}
