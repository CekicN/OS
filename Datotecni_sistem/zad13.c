/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu i njegovim poddirektorijumima, maksimalno do treceg nivoa, 
odredjuje i stampa imena svih regularnih datoteka koje su vece od 100KB, a manje od 750KB. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_NAME 1024

void processdir(char * path, int level);

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
		printf("Doslo je do greske prilikom ocitavanja statusa unete putanje\n");
		exit(-1);
	}

	//da li je uneta putanja do direktorijuma
	if (!S_ISDIR(statbuf.st_mode))
	{
		printf("Uneti argument ne predstavlja putanu do direktorijuma\n");
		exit(-1);
	}

	processdir(dirname, 1);

	return 0;
}
void processdir(char * path, int level)
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
		if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
			continue;

		strcpy(path1, path);
		strcat(path1, "/");
		strcat(path1, dirp->d_name);

		stat(path1, &statbuf);

		if (S_ISREG(statbuf.st_mode) && statbuf.st_size > 100 * 1024 && statbuf.st_size < 750 * 1024)
		{
			printf("Datoteka: %s\n", dirp->d_name);
		}
		else if (S_ISDIR(statbuf.st_mode) && level < 3)
		{
			processdir(path1, level + 1);
		}
	}
	closedir(dp);
}