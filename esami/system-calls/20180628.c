#include <fcntl.h> //per operazioni sui file (open)
#include <stdio.h>
#include <unistd.h> //chiamate di base UNIX (fork)
#include <signal.h> //per i segnali
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int pfd[2]; 
int pid[2]; 
int fdin; 
int righe = 0;

int main(int argc, char* argv[]){
    if(argc != 4){
        fprinf(stderr, "Numero di argomenti errato\n");
        fprintf(stderr, "Usage: \n\t%s Fin N C\n" argv[0]);
        exit(1);
    }

    int Tout; 
    Tout = atoi(argv[2]);

    if(Tout < 0){
        fprinf(stderr, "Secondo argomento non è un un intero positivo\n");
        exit(1);
    }

    if(pipe(pfd) < 0){
        fprintf(stderr, "Errore nella creazione della pipe tra P0 e P1\n"); 
        exit(1);
    }

    pid[0] = fork();
    if(pid[0] == 0){ //P1 -> figlio
    signal(SIGUSR1, p1_handler);
    printf("P1, pid = %d\n",getpid());
    close(pfd[0]); //chiusura lato lettura
    processoP1(argv[1]);
    exit(EXIT_SUCCESS);
    }
    else if(pid[0] > 0){ // P0 -> padre
        pid[1]=fork();
        if(pid[1]==0){//P2
            printf("P2, pid = %d\n",getpid());
            close(pfd[1]);//chiudo lato di scrittura
            processoP2(argv[3]);
            exit(EXIT_SUCCESS); 
        }
        else if(pid[1]>0){//P0
            signal(SIGALRM, p0_alrm_handler);
        	alarm(Tout);
            close(pfd[0]);//chiudo lato di lettura
            close(pfd[1]);//chiudo lato di scrittura
            while(1){
				printf("P0: PID=%d\n",getpid());
				sleep(1);
			}
        }
        else{
            perror("P0: errore nella fork per P2\n");
            exit(EXIT_FAILURE); 
        }
    }
    else {
       perror("P0: errore nella fork per P1\n");
       exit(EXIT_FAILURE); 
    }
}

void p1_handler(int sigNo){
    if(signo == SIGUSR1){
        printf("P1: ho ricevuto il segnale SIGUSR1\n");
        close(pfd[1]); //chiude lato scrittura
        close(fdin);        
        exit(EXIT_SUCCESS);
    }
    else{
    perror("P1: segnale inaspettato\n");
    exit(EXIT_FAILURE);
    }
}

void processoP1(char* filename){
    int seek_val; 
    int nread; 
    int written; 
    char temp;

    fdin=open(filename, O_RDONLY);
    if(fdin < 0){ 
        fprintf(stderr, "P1: errore nell'apertura file %s. Termino.\n", filename);
        exit(EXIT_FAILURE);
    }

    while(1){
        seek_val = lseek(fdin, -1*sizeof(char), SEEK_END);
        if(seek_val < 0){
            perror("P1: errore nella lettura del file di input\n");
            close(fdin); 
            exit(EXIT_FAILURE);
        }

        nread = read(fdin, &temp, sizeof(char)); //lettura del file

        while(nread > 0 && lseek >= 0){
            if(temp == '\n'){
                righe++;
            }
            written = write(pfd[1], &temp, sizeof(char));
            if(written < 0){
                perror("P1: errore scrittura su pipe. Termino\n");
                exit(EXIT_FAILURE);
            }
            seek_val = lseek(fdin, -2*sizeof(char), SEEK_CUR);
        }
        sleep(1);
    }
}

void p0_alrm_handler(int signo){
	int i;
	if (signo == SIGALRM){
		printf("P0: mando segnale a P1\n");
		kill(pid[0], SIGUSR1);
		// aspetto i figli e controllo il loro stato di terminazione in modo da 
    	//riscontrare eventuali loro malfunzionamenti
    	for(i=0;i<2;i++)
        	wait_child(); 
        printf("P0: termino.\n");
        exit(EXIT_SUCCESS);
	}else {
		perror("P0: segnale inatteso.\n");
		exit(EXIT_FAILURE); 
	}
}

void processoP2(char *fileout){
	close(0);
    dup(pfd[0]);
    close(1);
    int fdout=open(fileout,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    //int fdout=open(fileout,O_WRONLY);
    if(fdout<0){
        fprintf(stderr,"P2: Errore apertura file %s. Termino.\n",fileout);
        exit(1); 
    }
    execlp("cat", "cat", (char*)0);
    perror("P2: Errore nella exec. Termino.\n");
    exit(EXIT_FAILURE);
}

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
