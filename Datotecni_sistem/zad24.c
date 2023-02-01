/*Na programskom jeziku C napisati Linux/UNIX program koji u zadatom folderu. Putanja (apsolutna
ili relativna) do foldera se zadaje kao argument komandne linije, a program treba da odštampa broj
regularnih datoteka koje su veće od 30KB.*/

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
int main(int argc, char* argv[])
{
 DIR* dp;
 struct dirent* dirp;
 int brdat;
 struct stat statbuf;
 int result;
 if (argc != 2)
 {
 printf("Neophodan je jedan argument (putanja do foldera)!\n");
 exit(1);
 }
 if ((dp = opendir(argv[1])) == NULL)
 {
 printf("Greska prilikom otvaranja foldera %s!\n", argv[1]);
 exit(1);
 }
 brdat = 0;
 while( (dirp = readdir(dp)) != NULL )
 {
 if ((result = stat(dirp->d_name, &statbuf)) == -1)
 {
 printf("Neuspesno citanje podataka o objektu: %s\n", dirp->d_name);
 continue;
 }
 if(S_ISREG(statbuf.st_mode) && statbuf.st_size > 30 * 1024) {
 brdat++;
 }
 }
 printf("Folder %s sadrzi %d reg. datoteka vecih od 30KB.\n", argv[1], brdat);
 closedir(dp);
}