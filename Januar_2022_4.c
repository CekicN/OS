#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>

int i = 1;
void obidji(char* ime, int n, int m)
{
    DIR* dp = opendir(ime);
    if(dp == NULL)
    {
        printf("Greska pri otvaranju direktorijuma!");
        exit(1);
    }
    struct dirent* dir;
    struct stat st;

    char path[100];
    while((dir = readdir(dp)) != NULL)
    {
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 )
            continue;

        strcpy(path, ime);
        strcat(path, "/");
        strcat(path, dir->d_name);

        stat(path, &st);
        
        if(S_ISREG(st.st_mode) && st.st_size > n && i > m)
        {
            printf("PATH: %s, SIZE: %ld\n", path, st.st_size);
        }
        else if(S_ISDIR(st.st_mode))
        {
            i++;
            obidji(path, n,m);
        }

    }
    closedir(dp);

}
int main(int argc, char* argv[])
{

    obidji(argv[1], atoi(argv[2]), atoi(argv[3]));
    return 0;
}
