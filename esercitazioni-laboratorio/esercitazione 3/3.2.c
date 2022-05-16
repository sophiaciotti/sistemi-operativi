#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#define compito1 0
#define compito2 1

// sintassi: es3 N

int N,i;
int compito=-1; //compito per P2
float S=0.0; //variabile per sommatoria
int pid1, pid2, pid3;
int terminati=0;
    
void print_usage(char* prog_name){
    fprintf(stderr, "Usage:\n\t%s N\n", prog_name);
}

void P0_handler(int signo){
    int status, pid;
    terminati++;
    printf("P0 (%d): ricevuto sigchld: un figlio è terminato.\n",getpid());
    pid=wait(&status);
    if ((char)status==0) 
        printf("P0: il figlio %d è terminato volontariamente con stato %d\n",pid, status>>8);
    else 
        printf("P0: il figlio  %d è  terminato involontariamente per segnale %d\n",pid, (char)status);
    if  (terminati==2) //entrambi i figli sono terminati
    {   printf("P0: ho finito.\n");
        exit(0);
    }
}

void P1_handler(int signo){
    int status, pid;

    printf("P1 (%d): ricevuto segnale %d: uccido P3....\n",getpid(), signo);
    kill(pid3, SIGKILL);
    pid=wait(&status);
    if ((char)status==0) 
        printf("P1: il figlio P3(%d) è terminato volontariamente con stato %d\n",pid, status>>8);
    else 
        printf("P1: il figlio  P3(%d) è  terminato involontariamente per segnale %d\n",pid, (char)status);
    printf("P1: ho finito.\n");
    exit(0);
}

void P2_handler(int signo){
    if (signo==SIGUSR1){
        printf("P2 (%d): ricevuto SIGUSR1\n", getpid());
        compito=compito1;
    }else if (signo==SIGUSR2){
        printf("P2 (%d): ricevuto SIGUSR2\n", getpid());
        compito=compito2;
    }else if (signo==SIGALRM){ //Dopo N secondi P2 stamperà il valore di S e di i; poi terminerà restituendo al proprio padre il valore 0.
        printf("P2(%d): raggiunto il timeout di %d secondi. Ecco i valore di S dopo %d iterazioni: %f\n", getpid(), N,i,S);
        kill(pid1, SIGUSR1);
        exit(0);
        }
}

int main(int argc, char* argv[])
{   // Controllo argomenti:    
    if (argc != 2) {
        fprintf(stderr, "Errore nel numero di argomenti\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    N = atoi(argv[1]);
    if (N<0){
        fprintf(stderr, "Il parametro N deve essere un intero positivo\n");
        exit(EXIT_FAILURE);
    }
    signal(SIGCHLD, P0_handler); //solo per P0
    // creazione P1
    pid1 = fork();
    if (pid1==0){ //P1
            signal(SIGCHLD, SIG_DFL);
            signal(SIGUSR1, P1_handler); //solo per P1
            pid3=fork(); //creazione P3
            if (pid3==0) //nipote P3: esegue top
            {   execlp("top","top",(char *)0);
                perror("P3: top fallito -");
                exit(-1); 
            }
            //continua  P1:
            pause();
            exit(-1);
    }// fine P1
    //continua P0:
    signal(SIGUSR1, P2_handler); //solo per P2
    signal(SIGUSR2, P2_handler); //solo per P2
    signal(SIGALRM, P2_handler); //solo per P2
    pid2=fork();//creazione P2
    if (pid2==0) { //P2
        pause();//attesa segnale da P0
        alarm(N);//impostazione timeout
        for (i=0;;i++)  // ciclo di calcolo
        {   if (compito==compito1)
                S+=1/(i+1);
            else  if (compito==compito2)
                S+=(i+2)/(i+1);
        }
        exit(-1); //non dovrebbe servire
    }
    
    //P0:
    sleep(1);
    if (getpid()%2==0)
            kill(pid2, SIGUSR1);
    else
            kill(pid2,SIGUSR2);
    for(int i=0;;i++){
        sleep(2);
        printf("P0 (%d)\n", getpid());
    }
}// fine main
