/*Koriscenjem programskog jezika C napisati Linux program koji kreira dva dodatna procesa. 
Proces roditelj sa procesima decom komunicira koriscenjem signala SIGUSR1 i SIGUSR2. 
Na svakih 10 sekundi, proces roditelj salje signal SIGUSR1 svakom procesu detetu. 
Kada procesi deca prime signal SIGUSR1, pristupaju generisanju tekstualnih datoteka sa podacima. 
Prvi proces dete generise datoteku prva.txt koja sadrzi 1000 slucajnih celih brojeva, 
a drugi proces dete generise datoteku druga.txt koja sadrzi 1000 slucajnih malih slova. 
Svaki proces dete, kada zavrsi sa generisanjem datoteke, obavestava o tome proces roditelj
slanjem signala SIGUSR2. Kada proces roditelj primi signale SIGUSR2 od oba deteta, na standardni
izlaz stampa sadrzaje obe datoteke. Citav postupak se ponavlja 20 puta. */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void makePrva(){
    FILE* f = fopen("prva.txt", "w");
    for(int i = 0; i < 2; ++i)
        fprintf(f, "%d\n", rand() % 100);
    fclose(f);
}

void makeDruga(){
    FILE* f = fopen("druga.txt", "w");
    for(int i = 0; i < 2; ++i)
        fprintf(f, "%c\n", rand() % 25 + 97);
    fclose(f);
}

void printAll(){
    FILE* f = fopen("prva.txt", "r");
    if(f == NULL){
        printf("Prva.txt ne postoji.\n");
        exit(1);
    }
    char buf[100];
    while(!feof(f)){
        fgets(buf, 100, f);
        printf("%s", buf);
    }
    f = fopen("druga.txt", "r");
    if(f == NULL){
        printf("Druga.txt ne postoji.\n");
        exit(1);
    }
    while(!feof(f)){
        fgets(buf, 100, f);
        printf("%s", buf);
    }
    fclose(f);
}
 
int main(int argc, char const *argv[])
{
  
    int pid1;
    int pid2;
 
    if((pid1 = fork()) == 0){
        for(int i = 0; i < 2;i++){
            signal(SIGUSR1, makePrva);
            pause();
            //sleep(1);
            kill(getppid(), SIGUSR2);
        }
    }
    else if((pid2 = fork()) == 0){
        for(int i = 0; i < 2;i++){
            signal(SIGUSR1, makeDruga);
            pause();
            //sleep(2);
            kill(getppid(), SIGUSR2);
        }
    }
    else{
        for(int i = 0; i < 2;i++)
        {
            sleep(1);
            kill(pid1, SIGUSR1);
            kill(pid2, SIGUSR1);
            signal(SIGUSR2, printAll);
            
            for(int i = 0; i < 2; ++i)
                pause();
            sleep(1);

        }
    }
}