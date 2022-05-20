#include <fcntl.h> //per operazioni sui file (open)
#include <stdio.h>
#include <unistd.h> //chiamate di base UNIX (fork)
#include <signal.h> //per i segnali
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h> //per contggio secondi

int p1; 
int p2; 
int N;

//si occupa del job di P2: uccidere P0 -> viene chiamato dopo che sono passati N secondi
void p2alarm_handler(int sig){
    printf("P2: timeout exeeded, killing P0\n");
    kill(getppid(), SIGKILL); //get parent pid, invia segnale di kill
    exit(0); //exit success
}

int main(int argc, char *argv[]) {

    //The first element of the pip array, pipe[0] is used for reading data from the pipe.
    //The second element of the pip array, pipe[1] is used for writing data to the pipe.
    int pip[2];
    int i; 
    int count; 
    int fd; 
    int oldCount; 
    int diff; 
    int nread; 
    char filecontent[1025];
    char c; 

    //controllo parametri
    if(argc != 4){
        perror("Numero di parametri errato \n");
        exit(-1); //exit error
    }

    //il primo identificativo è un file assoluto -> Fin
    if(argv[1][0] != '/'){
        printf("Il primo argomento assoluto di un file deve essere uno / \n");
        exit(-2); //exit error
    }

    int W = atoi(argv[2]);
    if(W <= 0){
        printf("Il secondo argomento deve essere un intero positivo / \n");
        exit(-3); //exit error, -3 = doveva essere un numero intero positivo
    }

    int T = atoi(argv[3]);
    if(t <= 0){
        printf("Il terzo argomento deve essere un intero positivo / \n");
        exit(-3); //exit error -> ogni tipo di errore è identificato dal numero
    }

    //pipe viene usata per aprire file descriptors 
    //se minore di zero -> errore nell'inizializzazione (successo pipe = 0)
    if(pipe(pip) < 0){
        perror("Errore pipe\n"); 
        exit(1);
    }

    p1=fork();

    //con p1 = 0 il processo viene passato al figlio
    //con p1 < 0 fork non è andato a buon fine 
    //con p1 > 0 il processo viene passato al padre
    // -> bisogna creare il codice per ogni caso sopra elencato
    if(p1==0){
        close(pip[1]); //chiude canale di scrittura
        i = 0;
        oldCount = 0;

        //lettura file, se la lettura va a buon fine
        while(read(pip[0], &count, sizeof (int)) > 0){
            diff = count -oldCount;
            if(diff > 0){
				printf("Lettura numero %d: trovati %d caratteri aggiuntivi\n", i, diff);              
            }
            if (diff<0){
				printf("Lettura numero %d: %d caratteri cancellati\n", i, diff*(-1));
            }
            i++; 
            oldCount = count;
        }
        printf("Lettura terminata \n"); 
        exit(0);
    }

    //con p1 > 0 il processo viene passato al padre
    else if (p1 > 0){
        //creazione processo p2
        p2 = fork(); 
        if(p2 == 0){
            close(pip[1]); //chiude canale scrittura
            close(pip[0]); //chiude canale lettura
            signal(SIGALRM, p2alarm_handler); //attende segnale che viene gestito dall'handler
            alarm(T); //impostazione allarme dopo che sono passat T secondi
            
            //stampa sempre ogni 2 secondi
            while(1){
                sleep(2);
                printf("P2: mio pid %d\n", getpid());
            }
        }
        //passa al processo padre
        else if(p2 > 0){
            //open minore di zero -> errore
            if((fd = open(argv[1], O_RDONLY)) < 0){
                perror("Errore open\n");
                exit(-1);
            }
            //altrimenti, 
            while(1){
                count = lseek(fd, 0, SEEK_END); //lseek is a system call that is used to change 
                //the location of the read/write pointer of a file descriptor. The location can be set either in absolute or relative terms
                printf("P0: caratteri sono %d\n", count);
                write(pip[1], &count, sizeof(int));
                sleep(W);
            }
        }

        else{
            printf("P0: Impossibile creare processo figlio P2\n");
			exit(3);
        }
    }
    
    else{
		printf("P0: Impossibile creare processo figlio P1\n");
		exit(3);       
    }
}
