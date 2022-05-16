#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_STRING_LENGTH 255
#define MAXP 20

int num_files;
int cerca_indice(int *pid, int p);
void usage(char* prog_name);

int main (int argc, char* argv[]){
    int i, k;
    int pid[MAXP];
    
    char buff[MAX_STRING_LENGTH];/* Buffer temporaneo dove, per comodità scrivo i messaggi di errore prima di stamparli */
    char dir1[MAX_STRING_LENGTH];
    char dir2[MAX_STRING_LENGTH];
    
    
    if ( argc <= 1 ) {/*Almeno un file da compilare*/
        fprintf(stderr,"Numero di argomenti non valido\n");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    strcpy(dir1, argv[1]);
    strcpy(dir2, argv[2]);
    printf("directory di partenza: %s\n", dir1);
    printf("directory di destinazione: %s\n", dir2);
    
    num_files = argc - 3;

    for( i=0; i<num_files; i++ ){
        pid[i] = fork();
        if ( pid[i] > 0 ) { /* Processo padre*/
            printf("Padre %d: creato figlio %d con pid: %d\n", getpid(), i, pid[i]);
        }
        else if ( pid[i] == 0 ) { //Processo figlio 
            int mypid=getpid();
             // "Costruisco" il nome assoluto del file
            char fileI[MAX_STRING_LENGTH]; 
            strcpy(fileI, dir1);
            strcat(fileI, "/");
            strcat(fileI, argv[i+3]);
            
            if (mypid%2==0) // pid pari -> copia
            {   printf("\nFiglio %d (pid %d): ho il pid pari e copio il file %s nella directory %s \n", i, getpid(), fileI, dir2);
                execlp("cp", "cp", fileI, dir2,(char*)0 );
                sprintf(buff, "Figlio %d: impossibile copiare il file %s", i, fileI);
                perror(buff);
                exit(EXIT_FAILURE); 
            }
            else // pid dispari -> cancellazione
            {   printf("\nFiglio %d  (pid %d): ho il pid dispari ed elimino il file %s  \n", i, getpid(), fileI);
                execlp("rm", "rm", fileI, (char*)0 );
                sprintf(buff, "Figlio %d: impossibile rimuovere il file %s", i, fileI);
                perror(buff);
                exit(EXIT_FAILURE); 
            }
        }
        else { //padre
            sprintf(buff,"Padre: impossibile fare fork %d",i);
            perror(buff);
            exit(EXIT_FAILURE);
        }
    }

    
    for ( i=0; i<num_files; i++ ){
        int status, terminated_pid;
        terminated_pid = wait(&status);
        k=cerca_indice(pid, terminated_pid);
        if (WIFEXITED(status)) 
        {    if ( ! (WEXITSTATUS(status) == EXIT_SUCCESS) ){  
            fprintf(stderr,"Padre: il figlio con PID=%d  è fallito.\n", terminated_pid);
            }
        }
        else {
            printf("term. involontaria del figlio (pid= %d, file= %s per segnale %d\n",terminated_pid, argv[k+1], (char)status);
}
        
    }
    printf("\nPadre: ecco il contenuto della directory %s: \n", dir2);
    execlp("ls", "ls", "-l", dir2, (char*)0);
    perror("ls del padre fallita -");
    return EXIT_FAILURE; // se il padre arriva qui, l'execlp è fallita
}


int cerca_indice(int *pid, int p){
    int i;
    for(i=0;i<num_files; i++)
        if (pid[i]==p)
            return i;
    return -1; 
}

void usage(char* prog_name){
    fprintf(stderr,"Usage:\n\t%s dir1 dir2 file1 [file2 ... ] \n", prog_name);
}
