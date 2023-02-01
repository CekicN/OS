/*Korišćenjem programskog jezika C napisati Linux/UNIX program koji prebrojava linije u
tekstualnoj datoteci čije je ime zadato kao argument komandne linije. Korišćenjem signala obezbediti
da program prekine izvršenje ukoliko se linije ne prebroje u roku od 2 sekunde*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
/* Funkcija za obradu signala */
void catch_alarm(int sig_num)
{
 signal(SIGALRM, SIG_DFL);
 printf("Brojanje traje predugo!\n");
 fflush(stdout);
 exit(1);
}
int main(int argc, char* argv[])
{
 /* Postavlja funkciju catch_int da obradjuje signal SIGALRM */
 signal(SIGALRM, catch_alarm);
 char line[100];
 int linecount = 0;
 alarm(2);
 FILE *f = fopen(argv[1], "r");
 while (!feof(f)){
 fgets(line, 100, f);
 linecount++;
// Namerno produzujem vreme brojanja linija
 sleep(1);
 }
 fclose(f);
 printf("Datoteka sadrzi %d linija.\n", linecount);
}
