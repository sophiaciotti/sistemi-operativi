#!/bin/bash
# esercitazione6.1.sh 

if test $# -ne 1 #numero parametri minore di 1 -> se true -> errore + exit
then echo Errore nel numero di parametri 
        exit
fi
S=$1  #S è una stringa di caratteri
echo Immetti il nome assoluto di un file esistente e leggibile #output da far leggere all'utente
read F #leggi il file che si chiama come il nome inserito dall'utente

#gestione dei diversi casi della lettura di F
case $F in  #caso in cui F non inizi per / -> no nome assoluto  
  /*) ;;    
  *)  echo $F non è un nome assoluto         
      exit 1;;
  esac

if [ ! -f $file ]; then #caso in cui F non sia un file
  echo $F non è un file
  exit 2
fi

if [ ! -r $file ]; then #caso in cui F non può essere letto
  echo $F non è leggibile
  exit 3
fi

OUT=$HOME/results_$USER.out  #nome del file di ouput
echo il file di output si chiama $OUT #echo non richiesto, solo per controllo

cat $F | grep $S | sort -r > $OUT 
#cat $F > $OUT -> scrive il contenuto di $F su $OUT
#grep $S [$F] -> rierca in $F le stringhe con $S 
#sort ordina il contenuto
