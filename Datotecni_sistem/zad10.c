/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu rekurzivno u svim njegovim poddirektorijumima
odredjuje sve regularne datoteke cija je velicina veca od N bajtova i cija je apsolutna putanja duza od M karaktera. 
N i M se takodje zadaju kao ulazni argumenti programa. Program na standardnom izlazu stampa apsolutne putanje do datoteka
i njihove velicine u bajtovima. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_NAME 1024

void processdir(char * path, int N, int M);

int main(int argc, char * argv[])
{

	char dirname[MAX_NAME];
	char path[MAX_NAME];
	struct stat statbuf;
	DIR * dp;
	int N,M;
	struct dirent * dirp;
	int count, size;

	if (argc < 2)
	{
		printf("Nema dovoljno ulaznih argumenata\n");
		exit(-1);
	}

	strcpy(dirname, argv[1]);
	N = atoi(argv[2]);
	M = atoi(argv[3]);
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

	processdir(dirname, N, M);

	return 0;
}
void processdir(char * path, int N, int M)
{
	DIR * dp;
	struct dirent * dirp;
	char path1[MAX_NAME];
	struct stat statbuf;
	dp = opendir(path);
    printf("Otvoreno: %s\n", path);
	
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

		if (S_ISREG(statbuf.st_mode) && statbuf.st_size > N && strlen(path1) > M)
		{
			printf("Fajl %s je veci od %d bajtova\n", path1,N);
		}
		else 
			processdir(path1, N, M);
	}
	closedir(dp);
}