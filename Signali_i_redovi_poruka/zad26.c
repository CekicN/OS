/*Korišdenjem programskog jezika C napisati Linux/UNIX program koji sa tastature učitava username i
zatim ga korišdenjem redova poruka šalje drugom procesu. Drugi proces za uneti username prvom
procesu vrada password (drugi proces za username pera vrada password peric, a za svaki drugi
username vrada NOT AVAILABLE). Zadatak rešiti korišdenjem JEDNOG reda poruka. */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <string.h>
#define RED_PORUKA 10002
#define MAX_PORUKA 255
#define PROC_A_KEY 10101
#define PROC_B_KEY 10102
#define PROC_C_KEY 10103
struct poruka
{
long tip;
char tekst[MAX_PORUKA];
};
int main(int argc, char * argv[])
{
int pid;
int redid;
struct poruka bafer;
pid = fork();
if (pid < 0)
{
printf("Doslo je do greske prilikom kreiranja novog procesa\n");
exit(1);
}
if (pid == 0)
{
redid = msgget(RED_PORUKA, IPC_CREAT | 0666);
if (redid < 0)
{
printf("Doslo je do greske prilikom povezivanja na red poruka!!!\n");
exit(1);
}
printf("Unesite username: ");
gets(bafer.tekst);bafer.tip = 1;
if (msgsnd(redid, &bafer, sizeof(bafer.tekst), 0) < 0)
{
printf("Doslo je do greske prilikom slanja poruke.\n");
exit(1);
}
if (msgrcv(redid, &bafer, MAX_PORUKA, 2, 0) < 0)//Citaju se samo poruke tipa 2
{
printf("Doslo je do greske prilikom prijema poruke.\n");
exit(1);
}
printf("Password: %s\n", bafer.tekst);
fflush(stdout);
} else {
redid = msgget(RED_PORUKA, IPC_CREAT | 0666);
if (redid < 0)
{
printf("Doslo je do greske prilikom povezivanja na red poruka!!!\n");
exit(1);
}
if (msgrcv(redid, &bafer, MAX_PORUKA, 1, 0) < 0)//Citaju se samo poruke tipa 1
{
printf("Doslo je do greske prilikom prijema poruke.\n");
exit(1);
}
bafer.tip = 2;
if (strcmp(bafer.tekst,"pera") == 0)
strcpy(bafer.tekst,"peric");
else
strcpy(bafer.tekst,"NOT AVAILABLE");
if (msgsnd(redid, &bafer, sizeof(bafer.tekst), 0) < 0)
{
printf("Doslo je do greske prilikom slanja poruke.\n");
exit(1);
}
}
//Red poruka se brise tek kada se zavrsi proces dete
wait(NULL);
msgctl(redid, IPC_RMID, 0);
return 0;
}