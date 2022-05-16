//funzione usage -> per stampare sul flusso d'errore standard l'identificativo con il quale si è verificato (nella verifica dei parametri: numeri e tipo)
void usage(char* prog_name){
  fprintf(stderr,"Usage:\n\t%s: stud1 stud2 .. studM \n", prog_name); 
}

//cerca indice per trovare figlio
int cerca_indice(int *pid, int p){
    int i;
    for(i=0;i< argomento ; i++)
        if (pid[i]==p)
            return i;
    return 0;
}

//funzione figlio, dopo il fork calcola quello richiesto -> utile per dividere i compiti quando i figli devono fare le stesse cose
void figlio(int indice){
 /* int i; 
  int somma = 0; 

  for(i=0, i< argomento, i++){
    somma = somma + M[indice][i];
  }
  */
  exit(somma); //perchè?
}

//utilizzo di handler quando i figli hanno compiti diversi

//compito del figlio cambia in base alla ricezione di diversi segnali
void P2_handler(int signo){
  if (signo==SIGUSR1){
    printf("P2 (%d): ricevuto SIGUSR1\n", getpid());
    compito=compito1;
  }else if (signo==SIGUSR2){
    printf("P2 (%d): ricevuto SIGUSR2\n", getpid());
    compito=compito2;
  }else if (signo==SIGALRM){ //Dopo N secondi P2 stamperà il valore di S e di i; poi terminerà restituendo al proprio padre il valore 0.
    printf("P2(%d): raggiunto il timeout di %d secondi. Ecco i valore di S dopo %d iterazioni: %f\n", getpid(), N,i,S);
    kill(pid1, SIGINT);
    exit(0);
  }
}



