#include <stdio.h>
#include <string.h>
#include <ctype.h> //dichiara funzioni utilizzate per la classificazione dei caratteri

//funzione per la scrittura degli identificativi sul file
void usage(char* prog_name){
  fprintf(stderr,"Usage:\n\t%s: stud1 stud2 .. studM \n", prog_name); 
}

int main(int argc, char* argv[]){

  //controllo che sia stato passato almeno un identificativo, n° identificativo = argc
  //argc senza che gli sia passato alcun dato è già 1 perchè considera anche il nome del programma
  if(argc == 1){
    prinf("Sintassi errata.\n");
    usage(argv[0]); 
    return 0;
  }

  //controllo lunghezza stringa di ogni identificatore
  for(int i = 1; i < argc; i++){
    if(strlen(argv[i]) != 9){
      printf("Errore: la lunghezza di %s non è 9 caratteri\n", argv[i]);
      return 0;
    }
  }

  for(int k = 1; k < argc; k++){

    //controllo che ogni identificatore abbia come primi 3 caratteri quelli dei corsi
    if(strncmp(argv[k], "MED", 3) && strncmp(argv[k], "ING", 3) && strncmp(argv[k], "ECO", 3)){
    //la strncmp ritorna 0 solo quando le stringhe sono uguali, motivo per cui usiamo && al posto di ||
      printf("Errore: il corso di %s non è riconosciuto\n", argv[k]);
      return 0;
    }

    //controllo che ogni identificatore abbia come ultimi 6 caratteri delle cifre
    for(int j = 3; j < 9; j++){
      if(!isDigit(argv[k][j])){
        printf("Errore: la matricola %s ha il carattere %s che non è un numero.\n", argv[k], argv[k][j]);
        return 0;
      }
    }
  }

  //inizilizzazione contatori
  int nM=0; 
  int nI=0; 
  int nE=0;

  //conteggio per vedere quanti studenti ci sono per ogni corso
  for(int i = 1; i < argc; i++){
    if(strncmp(argv[i], "MED", 3)){
      nM++;
    }
    else if(strncmp(argv[i], "ING", 3)){
      nI++;
    }
    else if(strncmp(argv[i], "ECO", 3)){
      nE++;
    }
  }

  //stampa dei numeri di tutti i corsi
  printf("Medicina: %s\tIngegneria: %s\tEconomia: %s", nM, nI, nE);

  //stampa del corso con più iscritti + stama di tutte le matricole appartenenti al corso
  if(nM>=nI && nM>=nE){
    printf("Il corso con più iscritti è Medicina con %d studenti, ecco le matricole:\n", nM);
    for(int i=1; i<argc; i++){
      if(!strncmp(argv[i], "MED", 3)){
        for(int j = 3; j<9; j++){
          printf("%c", argv[i][j]);
          printf("\n");
        }
      }
    }
  }

  else if(nI>=nM && nI>=nE){
    printf("Il corso con più iscritti è Ingegneria con %d studenti, ecco le matricole:\n", nI);
    for(int i=1; i<argc; i++){
      if(!strncmp(argv[i], "ING", 3)){
        for(int j = 3; j<9; j++){
          printf("%c", argv[i][j]);
          printf("\n");
        }
      }
    }
  }

  else if(nE>=nM && nE>=nI){
    printf("Il corso con più iscritti è Economia con %d studenti, ecco le matricole:\n", nE);
    for(int i=1; i<argc; i++){
      if(!strncmp(argv[i], "ECO", 3)){
        for(int j = 3; j<9; j++){
          printf("%c", argv[i][j]);
          printf("\n");
        }
      }
    }
  }

  return 0;
}
