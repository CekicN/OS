#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int i;
void obidji(char* pat, int d)
{
    DIR* dp = opendir(pat);
    struct dirent* dir;
    struct stat statbuf;
    char path[100];

    int fd;
    while((dir = readdir(dp)) != NULL && (i == -1 || i+1 <= d))
    {
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        int j = 0;
        strcpy(path, pat);
        strcat(path, "/");
        strcat(path, dir->d_name);
        stat(path, &statbuf);
        char buf[100];
        int n;
        if(S_ISREG(statbuf.st_mode))
        {
            printf("U fajlu: %s\n", path);
           fd = open(path, O_RDONLY);
           while((n = read(fd, buf, 100)) > 0 && j < 10)
           {
               printf("%s\n", buf);
               j++;
           }
        }
        else if(S_ISDIR(statbuf.st_mode))
            obidji(path, d);
        if(d != 0)
            i++;
    }
    close(fd);
}
int main(int argc, char* argv[])
{
    if(atoi(argv[2]) == 0)
        i = -1;
    else
        i = 0;
    char pat[100];
    strcpy(pat, argv[1]);
    int d = atoi(argv[2]);

    obidji(pat, d);
    return 0;
}
