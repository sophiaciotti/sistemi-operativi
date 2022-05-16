#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define MAXG 7 //numero max giorni di lavoro per un rider
#define MAXN 30 //numero max rider per periodo

int M[MAXN][MAXG]; //inizializzare con valori max, nel caso siano meno si lasciano vuoti, è quello che deve calcolare il padre, ricompensa rider per giorno
int G; 
int N;

int main(int argc, char* argv[]){

  //controllo valori in ingresso 
  if(argc != 3){
    printf("%s: sintassi sbagliata - 2 argomenti interi (G e N) \n", argv[0]);
    exit(1); //terminazione del processo
  }

  //valore G e N che vengono da argv
  int G = atoi(argv[1]);
  int N = atoi(argv[2]);

  //controllo dei parametri N e G
  if(G>MAXG){
    printf("%s: sintassi sbagliata - G non è valido \n", argv[0]);
    exit(1); //terminazione del processo    
  }
  if(N>MAXN){
    printf("%s: sintassi sbagliata - N non è valido \n", argv[0]);
    exit(1); //terminazione del processo    
  }

  srand(time(NULL)); //metodo per numeri pseudocasuali

  //iniziaizzazione matrice 
  for (int i=0; i<N; i++ ) {
    for(int j=0; j<G; j++)
    M[i][j]=rand()%36; // %36 restringe il campo tra 0 e 35 
  }

  //stampa matrice 
  printf("\n ----- DATI -----\n");
  for (int i=0; i<N; i++ ) {
    printf("Compensi del rider %d:",  i);
    for(int j=0; j<G; j++){   
        printf("\t%d", M[i][j]);
    }
    printf("\n");
  }
  printf("----------------\n\n");

  //creazione dei figli da parte del padre 
  int pid[MAXN]; //array di dim 30

  for(int i=0; i<N; i++){
    pid[i]=fork(); //creazione processo figlio di indice i

    if(pid[i]== 0){ //fork() -> se il pid restituito è zero allora il processo viene svolto dal figlio
      figlio(i);
    }

    else if(pid[i]> 0){ //fork() -> se il pid restituito è maggiore di zero allora il processo viene svolto dal padre
      printf("%d: ho creato il figlio %d (PID %d)\n", getpid(),i, pid[i]); //l'ha creato con il fork alla r.58
    }

    else{
      perror("Fork error:"); //comicazione tipologia di errore
      exit(1); //terminazione per imprevisto
    }

  }

  int p;
  int status;
  int k;

  for (int i=0; i<N; i++ ) { // attesa figli
    p=wait(&status);
    k=cerca_indice(pid,p); // ricerca indice del figlio di pid p
    if ((char)status==0) 
      printf("\nIl compenso totale del rider n.%d (pid=%d) è %d\n",k,p, status>>8);
    else 
      printf("term. involontaria  per segnale %d\n",(char)status);
  } 
}

//funzione figlio -> calcola guadagno di ogni rider 
void figlio(int indice){
  int i; 
  int somma = 0; 

  for(i=0, i<G, i++){
    somma = somma + M[indice][i];
  }

  exit(somma);
}

int cerca_indice(int *pid, int p){
    int i;
    for(i=0;i<N; i++)
        if (pid[i]==p)
            return i;
    return 0;
}
