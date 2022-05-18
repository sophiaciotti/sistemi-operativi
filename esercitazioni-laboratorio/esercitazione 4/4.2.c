#include <signal.h> 	/* uso i segnali*/
#include <fcntl.h>	/* uso i file*/
#include <string.h>	/* per manipolare i messaggi*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define dim 11 /* dimensione buffer */

void print_usage(char* prog_name){
    fprintf(stderr, "Usage:\n\t%s Fe Fu G N\n", prog_name);
}
typedef struct{
    int giorno;
    int importo;
} info;

void figlio(int i, char *fileIn, int g, int n);  
void wait_child(int *p, int n);

int main(int argc, char *argv[])
{ 	int pid[2];
    int fd, G, N, fine=0;
    char C;
    char tipoOp;
    
    info S;
    
    if (argc!=5)
    {   print_usage(argv[0]);
        exit(-1);
    }
    //controllo degli argomenti N e G
    G=atoi(argv[3]);
    if (G<1 ||G>31)
    {   printf("%s non può essere un giorno\n", argv[3]);
        exit(-2);
    }
    N=atoi(argv[4]);
    if (N<0 ||N>999999)
    {   printf("%s non è un importo valido\n", argv[4]);
        exit(-3);
    }
    // controllo file:
    fd=open(argv[1], O_RDONLY);
    if (fd<0)
    {   perror("open Fe fallita: ");
        exit(-4);
    }
    close(fd);
    fd=open(argv[2], O_RDONLY);
    if (fd<0)
    {   perror("open Fu fallita: ");
        exit(-5);
    }
    close(fd);
   
    for (int i=0; i<2; i++)
    {   pid[i]=fork();
        if (pid[i]==0)
           figlio(i, argv[i+1], G, N);
    }
    for(int i=0; i<2; i++) //eseguito dal padre
        wait_child(pid,N);//aspetta ogni figlio e stampa
}//fine main

void figlio(int i, char *fileIn, int g, int n)
{   int fin, fout, k, fine=0, trovato=0, point;
    info S;
    char GIORNO[3]="";
    char IMPORTO[7]="";
    char eol='\n';
    fin=open(fileIn, O_RDONLY);
    if (i==0) //P1
        fout=creat("./temp1", 0777);
    else 
        fout=creat("./temp2", 0777);
    lseek(fin,-sizeof(info), SEEK_END); //posiziona I/O pointer sull'ultimo elemento
    while (!fine) 
    {   k=read(fin, &S, sizeof(info));
        if (k==sizeof(info)) // ho letto una nuova operazione
        {   if( S.giorno>=g ) //l'operazione è nell'intervallo
            {   if (S.importo>=n)
                {   printf("figlio %d:  giorno %d e importo %d soddisfano il criterio\n", i, S.giorno, S.importo);
                    sprintf(GIORNO, "%d", S.giorno);
                    sprintf(IMPORTO, "%d", S.importo);
                    write(fout, GIORNO, strlen(GIORNO));
                    write(fout,&eol,1);
                    write(fout, IMPORTO, strlen(IMPORTO));
                    write(fout,&eol,1);
                    trovato=1;
                    fine=1; //trovato, quindi termino.
                }
                else // l'importo è minore di N --> posiziono I/O pointer sul  record precedente
                {   point=lseek(fin, -2*sizeof(info), SEEK_CUR);
                    printf("figlio %d:  giorno ok (%d), importo no (%d) --> indietreggio al n. %d\n",i, S.giorno, S.importo, point);
                    if (point<0) fine=1; //SCANSIONE FINITA
                }
            }
            else //il giorno è minore di G --> non ho trovato nulla nell'intervallo
                fine=1;
        }
        else  
            fine=1;
    }
    close(fin);close(fout);
    if (trovato==1)
         exit(0);
    else{
         printf("figlio %d: non ho trovato nulla nell'intervallo dato.\n",i);
         exit(1);
     }
} /* fine figlio */

void wait_child(int p[], int n) {
    int pid_terminated,status, fd, k, fterm;
    char c;
    char fileris[dim];
    pid_terminated=wait(&status);
    if (pid_terminated==p[0])//p1
    {   fterm=0;
        strcpy(fileris, "./temp1");
    }
    else //p2
    {   fterm=1;
        strcpy(fileris,"./temp2");
    }
    if(WIFEXITED(status))
    {    printf("\nPadre P0: terminazione volontaria del figlio %d con stato %d \n",
                pid_terminated, WEXITSTATUS(status));
        if (WEXITSTATUS(status)==0)// ci sono dei risultati -->lettura file temp
        {   printf("Padre P0: ecco il risultato ottenuto dal figlio %d:\n",fterm);
            fd=open(fileris, O_RDONLY);
            while (k=read(fd, &c, 1)>0)
                write(1,&c,1);
            close(fd);
            unlink(fileris);
        }
        else
            printf("Padre P0: il figlio %d (pid %d) non ha trovato operazioni con importo maggiore di %d nell'intervallo dato.\n", fterm, pid_terminated, n);
    }
    else if(WIFSIGNALED(status))
        printf("\nPADRE: terminazione involontaria del figlio %d a causa del segnale %d\n",
                pid_terminated,WTERMSIG(status));
    return;
}
