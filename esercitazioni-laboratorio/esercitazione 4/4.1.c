#include <signal.h> 	/* uso i segnali*/
#include <fcntl.h>	/* uso i file*/
#include <string.h>	/* per manipolare i messaggi*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#define dim 11 /* dimensione buffer */

void print_usage(char* prog_name){
    fprintf(stderr, "Usage:\n\t%s Fin Fe Fu\n", prog_name);
}
typedef struct{
    int giorno;
    int importo;
} info;

int leggi_riga(int fd, char *op, int *giorno, int *importo) // legge la prossima riga dal file fd, estraendo op, giorno importo: torna 0 se tutto ok, 1 se il file è finito
{   char c, buf[dim]="";
    int i, fine;
    int k=read(fd, op,1);//lettura tipo operazione
    if (k==0) return 1;
    printf("la riga rappresenta un'operazione di tipo %c:\n", *op);
    read(fd,&c,1); // salto '-'
    
    for (i=0, fine=0; i<2 && !fine; i++)
    {   read(fd,&buf[i], 1);// leggo prossimo carattere
        if (buf[i]=='-') //separatore
        {   buf[i]='\0';
            fine=1;
        }
    }
    printf("Ho letto il giorno: %s \n", buf);
    *giorno=atoi(buf);
    if (!fine) // scarto il separatore
        read(fd,&c,1); // salto '-'
    for (i=0, fine=0; i<6 && !fine; i++)
    {   read(fd,&buf[i], 1);// leggo prossimo carattere
        if (buf[i]=='\n') //fine riga
        {   buf[i]='\0';
            fine=1;
        }
    }
    printf("Ho letto l'importo: %s \n", buf);
    *importo=atoi(buf);
    return 0;
}
    
int main(int argc, char *argv[])
{ 	
    int fd, fdIn, fdE,fdU, n, fine=0;
    char C;
    char tipoOp;
    
    info S;
    
    if (argc!=4)
    {   print_usage(argv[0]);
        exit(-1);
    }
    // apertura file:
    fdIn=open(argv[1], O_RDONLY);
    if (fdIn<0)
    {   perror("open Fin fallita: ");
        exit(-2);
    }
    fdE=creat(argv[2], 0777);
    if (fdE<0)
    {   perror("open Fe fallita: ");
        exit(-3);
    }
    fdU=creat(argv[3], 0777);
    if (fdU<0)
    {   perror("open Fu fallita: ");
        exit(-4);
    }
    fine=0;
    while (!fine) 
    {       fine=leggi_riga(fdIn, &tipoOp, &S.giorno, &S.importo);
            if (!fine) // in buf c'è una nuova riga di Fin
            {   
                if (tipoOp=='E')
                    fd=fdE;
                else if (tipoOp=='U')
                    fd=fdU;
                else
                {   printf("Errore in Fin, termino!");
                    close(fdIn);close(fdE); close(fdU);
                    exit(-5);
                }
                printf("Main - verifica campi di S: giorno %d, importo %d\n", S.giorno, S.importo);
                write(fd, &S, sizeof(info)); 
            }
		}	
        close(fdIn);close(fdE); close(fdU);
        exit(0);
	} /* fine main */
