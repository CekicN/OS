/*Korišćenjem programskog jezika C napisati Linux/UNIX program koji se ne može prekinuti
kombinacijom tastera Ctrl-C, već samo broji koliko puta je ova kombinacija tastera pritisnuta. Program
svakih 10sec na ekranu štampa koliko puta do tada je pritisnuta kombinacija tastera Ctrl-C. Po isteku
30sec program izlazi i štampa ukupan broj pritisaka kombinacije tastera Ctrl-C. U glavnom programu
simulirati neki vremenski zahtevan posao beskonačnom petljom.*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
/* Brojac Ctrl-C */
int ctrl_c_count = 0;
int alarm_counter = 0;
/* Funkcija za obradu Ctrl-C signala */
void catch_int(int sig_num)
{
 /* Ponovo postavlja signal handler */
 signal(SIGINT, catch_int);
 /* Inkrementira brojac Ctrl-C */
 ctrl_c_count++;
 printf("\nContinuing\n");
 fflush(stdout);
}
/* Alarm signal handler */
void catch_alarm(int sig_num)
{
 /* Ponovo postavlja signal handler */
 signal(SIGALRM, catch_alarm);
 /* Stampa Ctrl-C brojac */
 printf("\n\nDo sada, Ctrl-C je pritisnuto '%d' puta\n\n", ctrl_c_count);
 fflush(stdout);
 alarm(10);
 alarm_counter++;
 if (alarm_counter == 3)
 exit(1);
}
int main(int argc, char* argv[])
{
 /* Postavlja Ctrl-C and Ctrl-Z signal handlere */
 signal(SIGINT, catch_int);
 signal(SIGALRM, catch_alarm);
 alarm(10);
 /* Beskonacna petlja */
 for ( ;; )
 pause();
 return 0;
}