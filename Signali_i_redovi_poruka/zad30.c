/*Korišćenjem programskog jezika C napisati Linux/UNIX program koji prebrojava linije u
tekstualnoj datoteci čije je ime zadato kao argument komandne linije. Korišćenjem signala obezbediti
da se program ne može prekinuti kombinacijom tastera Ctrl-C.*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
/* Funkcija za obradu signala */
void catch_int(int sig_num)
{
 /* Postavlja se default funkcija za obradu signala za sledeci put kada bude pritisnuta kombinacija
Ctrl-C */
 signal(SIGINT, catch_int);
 printf("Nije moguce prekinuti program!\n");
 fflush(stdout);
}
int main(int argc, char* argv[])
{
 /* Postavlja funkciju catch_int da obradjuje signal SIGINT */
 signal(SIGINT, catch_int);
 char line[100];
 int linecount = 0;
 FILE *f = fopen(argv[1], "r");
 while (!feof(f)){
 fgets(line, 100, f);
 linecount++;
 sleep(1);
 }
 fclose(f);
 printf("Datoteka sadrzi %d linija.\n", linecount);
}


