/*Koriscenjem programskog jezika C napisati Linux program koji onemogucava da se tekuci proces prekine jednim pritiskom kombinacije tastera CTRL+C. 
Proces se prekida tek kada se ova kombinacija tastera pritisne dva puta.*/

#include <stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void catch_int(int sig_num)
{
    signal(SIGINT, SIG_DFL);
    printf("Nije moguce prekinuti program.\n");
    fflush(stdout);
}


int main(int argc, char* argv[])
{
    signal(SIGINT, catch_int);
    for(;;)
    pause();
}