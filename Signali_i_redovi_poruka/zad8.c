/* Koriscenjem programskog jezika C napisati Linux program koji omogucava da dva procesa komuniciraju
i sinhronizuju svoje izvrsavanje koriscenjem signala.Prvi proces po slucajnom principu kreira
1024 celobrojne vrednosti i smesta ih u binarnu datoteku prenos.dat. Kada izgenerise brojeve,
obavestava o tome drugi proces,slanjem signala SIGUSR1 i pauzira svoje izvrsavanje.Kada drugi
proces primi signal on sadrzaj datoteke prenos.dat prikazuje na standardnom izlazu,salje prvom
procesu signal SIGUSR2 i nakon toka pauzira svoje izvrsavanje.Po prijemu signala SIGUSR2,prvi 
proces ponovo startujr citav postupak.Postupak se ponavlja 4096 puta.Nakon toga se oba procesa zavrsavaju. */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void proccess1(){
    FILE *ptr;
    ptr = fopen("test.bin","wb"); 
    int niz[10];
    for(int i = 0; i < 10; i++){
        niz[i] = rand()%100;
    } 
    fwrite(&niz,sizeof(int),10,ptr);
    fclose(ptr);
}

void proccess2(){
    FILE* f = fopen("test.bin", "rb");
    
    int br[10],i = 0;
    fread(br,sizeof(int),10,f);
    for(int j = 0; j < 10;j++)
      printf("%d\n", br[j]);
    fclose(f);
}
 
int main(int argc, char const *argv[])
{
    int pid1;
    srand(time(NULL));
    if((pid1 = fork()) == 0)
    {
        for(int i = 0; i < 2; i++)
        {
            signal(SIGUSR2,proccess1);
            pause();
            kill(getppid(), SIGUSR1);
        }
    }
    else
    {
        
        for(int i = 0; i < 2; i++)
        {
            kill(pid1,SIGUSR2);
            signal(SIGUSR1, proccess2);
            pause();
        }
    }
}