/*Korišćenjem programskog jezika C napisati Linux program koji obilazi podstablo zadatog
direktorijuma i određuje naziv i putanju do datoteka čija je dužina 0B.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_NAME 1024
void processdir(char * path);
int main(int argc, char * argv[])
{
 char dirname[MAX_NAME];
 char path[MAX_NAME];
 struct stat statbuf;
 DIR * dp;
 struct dirent * dirp;
 int count, size;
 if (argc < 2)
 {
 printf("Nema dovoljno ulaznih argumenata\n");
 exit(-1);
 }
 strcpy(dirname, argv[1]);
 //proveravamo da li je uneta putanja validna
 if (stat(dirname, &statbuf) < 0)
 {
 printf("Doslo je do freske prilikom ocitavanja statusa direktorijuma\n");
 exit(-1);
 }
 //da li je uneta putanja do direktorijuma
 if (!S_ISDIR(statbuf.st_mode))
 {
 printf("Uneti argument ne predstavlja putanu do direktorijuma\n");
 exit(-1);
 }
 //posto smo utvrdili da se radi o direktorijumu pozivamo funkciju za obradu direktorijuma
 //funkcija ce rekurzivno obraditi citavo podstablo direktorijuma
 processdir(dirname);
 return 0;
}
void processdir(char * path)
{
 DIR * dp;
 struct dirent * dirp;
 char path1[MAX_NAME];
 struct stat statbuf;
 dp = opendir(path);
 if (dp == NULL) 
 {
 printf("Greska prilikom otvaranja direktorijuma\n");
 exit(-1);
 }
 while ((dirp = readdir(dp)) != NULL)
 {
 if (strcmp(dirp->d_name, ".") == 0
 || strcmp(dirp->d_name, "..") == 0)
 continue;
 strcpy(path1, path);
 strcat(path1, "/");
 strcat(path1, dirp->d_name);
 stat(path1, &statbuf);
 if (S_ISREG(statbuf.st_mode)
 && statbuf.st_size == 0)
 printf("Datoteka: %s\n", path1);
 else if (S_ISDIR(statbuf.st_mode))
 processdir(path1);

 }
 closedir(dp);
}