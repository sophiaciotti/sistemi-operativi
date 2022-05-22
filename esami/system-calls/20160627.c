#include <fcntl.h> //per operazioni sui file (open)
#include <stdio.h>
#include <unistd.h> //chiamate di base UNIX (fork)
#include <signal.h> //per i segnali
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h> //per contggio secondi

int pfd[2], pid[2];
char C[2];
char ftemp[15];

int main(int argc, char *argv[]){
    //controllo parametri

    if(argc != 4){
        fprinf(stderr, "Numero di argomenti errato\n");
        fprintf(stderr, "Usage: \n\t%s Fin N C\n" argv[0]);
        exit(1);
    }
    if(argv[1][0] != '/'){
        fprinf(stderr, "Primo argomento non è un file assoluto\n");
        exit(-2);
    }

    int N; 
    N = atoi(argv[2]);

    if(N < 0){
        fprinf(stderr, "Secondo argomento non è un un intero positivo\n");
        exit(1);
    }

    char C[2]; 
    if(strlen(argv[3] > 1)){
        fprinf(stderr, "C deve essere un carattere\n");
        exit(1);
    }

    C[0] = argv[3][0]; //primo carattere di FOUT -> file di testo non esistente -> è un carattere (vuoto?)
    C[1] = "\0"; //EOF -> il secondo carattere del file vuoto è EOF

    //uso sprintf per scrivere in un array di chars 
    char ftemp[15];
    sprintf(ftemp, "%s", "/tmp/temp"); //default nome file assoluto -> lo sta creando

    int pfd[2]; 
    if(pipe(pdf) < 0){
        fprinf(stderr, "Creazione pipe tra P0 e P1 non è andata a buon fine\n");
        exit(1);
    }

    //creazione P1
    int pid[2];
    pid[0]=fork(); 
    //gestione dei casi del fork per P1
    //con p1 = 0 il processo viene passato al figlio
    //con p1 < 0 fork non è andato a buon fine 
    //con p1 > 0 il processo viene passato al padre

    if(pid[0] == 0){ //P1
        printf("P1, pid = %d\n", getpid());
        close(pfd[0]); //chiudo lato di lettura 
        signal(SIGUSR1, p1_handler);
        pause();
        exit(0);
    }
    else if(pid[0] > 0){//P0 

        pid[1] = fork();

        if(pid[1] == 0) { //P2
        printf("P1, pid = %d\n", getpid());
        close(pfd[0]); //chiudo lato lettura 
        close(pfd[1]); //chiudo lato scrittura   
        
        processoP2(argv[[1], N]);
        exit(0);
        }
        else if(pid[1]>0){//P0
            close(pfd[1]);//chiudo lato di scrittura
            processoP0(pfd[0]);
        }
        else{
            fprintf(stderr,"P0: errore nella fork per P2\n");
            exit(1); 
        }
    }
    else{
       perror("P0: errore nella fork per P1\n");
        exit(1); 
    }
}

void p1_handler(int signo){
    int fdtemp;
    if(signo == SIGUSR1){
        printf("P1: ho ricevuto il segnale SIGUSR1\n");
        close(1); //chiude file descriptor in modo tale che si possa riutilizzare
        dup(pfd[1]); //creazione copia del file descritto
        execlp("grep", "grep", C , ftemp, (char *)0);
        perror("P1: errore nella exec\n");
        exit(3);
    }
    else("P1: segnale inaspettato\n"){
    exit(-2);
    }
}

void processoP2(char* fin, int N){
    int nread,fdtemp,fdin,written,crighe = 0;
    char temp;

    fdin=open(fin,O_RDONLY);
    if(fdin<0){
        fprintf(stderr,"P2: Errore apertura file %s\n",fin);
        exit(1); 
    }
    fdtemp=open(ftemp,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fdtemp<0){
        fprintf(stderr,"P2: Errore apertura file %s\n",ftemp);
        exit(1); 
    }
    while((nread=read(fdin,&temp,sizeof(char)))>0){
        if(crighe%N==0){
            written=write(fdtemp,&temp,sizeof(char));
            if(written<0){
                fprintf(stderr,"P2: Errore scrittura su file %s\n",ftemp);
                exit(1); 
            }
        }
        if(temp=='\n'){
            crighe++;   
        }
    }
    if(nread<0){
        fprintf(stderr,"P2: Errore nella lettura da file %s\n",fin);
        close(fdin);
        exit(1); 
    }
    kill(pid[0],SIGUSR1);
}

void processoP0(int pfd_in){
    int i,nread,fdtemp,fdin,written,crighe = 0;
    char temp;

    while((nread=read(pfd_in,&temp,sizeof(char)))>0){
        printf("%c",temp);
    }
    if(nread<0){
        perror("P0: Errore nella lettura da pipe");
        exit(1); 
    }
    // aspetto i figli e controllo il loro stato di terminazione in modo da 
    //riscontrare eventuali loro malfunzionamenti
    for(i=0;i<2;i++)
                wait_child(); 
}
/* codice alternativo per P0
void processoP0(int pfd_in){
    close(0);
    dup(pfd_in);
    execlp("cat", "cat", (char*)0);
    perror("P0: errore nella exec\n");
    exit(3);
    //NB: LA EXEC NON HA RITORNO SE NON IN CASO DI ERRORE. Dopo la exec c'è sempre una perror e una exit!
}*/

void wait_child() {
    int pid_terminated,status;
    pid_terminated=wait(&status);
    if(WIFEXITED(status))
        printf("P0: terminazione volontaria del figlio %d con stato %d\n",
                pid_terminated, WEXITSTATUS(status));
    else if(WIFSIGNALED(status))
        printf("P0: terminazione involontaria del figlio %d a causa del segnale %d\n",
                pid_terminated,WTERMSIG(status));
}
