#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_STRING_LENGTH 255
#define MAXP 20

int num_files;
void usage(char* prog_name);

int main (int argc, char* argv[]){
    int i, k;
    int pid[MAXP], nipote, terminated_pid, status;
    
    char buff[MAX_STRING_LENGTH];/* Buffer temporaneo dove, per comodità scrivo i messaggi di errore prima di stamparli */
    char dir1[MAX_STRING_LENGTH];
    char dir2[MAX_STRING_LENGTH];
    char fileI[MAX_STRING_LENGTH];
    char miopid[MAX_STRING_LENGTH];
    char newname[MAX_STRING_LENGTH];
    
    if ( argc <= 3 ) {/*Almeno un file */
        fprintf(stderr,"Numero di argomenti non valido\n");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    strcpy(dir1, argv[1]);
    strcpy(dir2, argv[2]);
    printf("directory di partenza: %s; directory di destinazione %s\n", dir1, dir2);
    
    num_files = argc - 3;

    for( i=0; i<num_files; i++ )
    {   //costruzione nome file I-simo (si trova in dir1):
        strcpy(fileI, dir1);
        strcat(fileI, "/");
        strcat(fileI, argv[i+3]);
       
        pid[i] = fork();
        if ( pid[i] > 0 ) { /* Processo padre*/
            printf("Padre %d: creato figlio %d con pid: %d\n", getpid(), i, pid[i]);
        }
        else if ( pid[i] == 0 ) 
        { /*Processo figlio adibito al mv */
            
            printf("Figlio %d: creo il processo nipote che dovrà copiare il file %s \n", i, fileI);
            nipote=fork(); // creazione NIPOTE copiatore
            if (nipote==0) //codice Nipote i-simo: deve eseguire cp
            {   printf("nipote %d (pid=%d) sto per copiare il file %s\n", i,getpid(), fileI);
                execlp("cp", "cp", fileI, dir2, (char*)0 );
                sprintf(buff, "nipote %d: impossibile copiare il file %s", i, fileI);
                perror(buff);
                exit(EXIT_FAILURE); 
            }
            else if (nipote>0) //CODICE FIGLIO i-simo: deve eseguire mv
            {   terminated_pid=wait(&status);
                if (WIFEXITED(status)) { // terminazione volontaria nipote copiatore          
                    if ( ! (WEXITSTATUS(status) == EXIT_SUCCESS) ){  // con fallimento
                        fprintf(stderr,"%d: il nipote copiatore con PID=%d  è fallito.\n",getpid(), terminated_pid);
                        exit(EXIT_FAILURE); // figlio termina per fallimento nipote
                    }
                }   
                else { // terminazione involontaria nipote copiatore 
                    printf("term. involontaria del figlio (pid= %d) per segnale %d\n",terminated_pid, (char)status);
                    exit(EXIT_FAILURE); // figlio termina per term involontaria nipote
                }
            // il nipote ha concluso volontariamente con successo -> figlio: mv
            k=getpid();
            sprintf(miopid,"%d",k); // miopid: stringa contenente il pid del processo corrente
            //costruzione nuovo nome:
            strcpy(newname, dir1);
            strcat(newname, "/");
            strcat(newname, miopid);
            
            execlp("mv","mv", fileI, newname, (char*)0);
            printf(buff, "figlio %d: impossibile rinominare il file %s", i, fileI);
            perror(buff);
            exit(EXIT_FAILURE); 
            
            }// fine nipote
    }
    }
    
    for ( i=0; i<num_files; i++ ){ //padre P0: attende tutti i figli
        terminated_pid = wait(&status); // attendo la terminazione dei figli
        if (WIFEXITED(status)) 
        {    if ( ! (WEXITSTATUS(status) == EXIT_SUCCESS) ){  
            fprintf(stderr,"Padre: il figlio con PID=%d  è fallito.\n", terminated_pid);
            }
        }
        else {
            printf("term. involontaria del figlio (pid= %d, file= %s per segnale %d\n",terminated_pid, argv[k+1], (char)status);
        }
    }
   
    return EXIT_SUCCESS;
}

void usage(char* prog_name){
    fprintf(stderr,"Usage:\n\t%s dir1 dir2 file1 [file2 ... ]\n", prog_name);
}
