#!/bin/bash

# esercitazione6.2.sh 

if test $# -lt 3
then echo Errore nel numero di parametri
        exit
fi
S=$1
shift
M=$1
shift
for i in $*
do
    case $i in    #verifico che sia un nome assoluto
    /*) ;;    
    *)  echo  $i non è un nome assoluto
        exit 1;;
    esac
    if [ ! -d $i ]
    then    echo Il  parametro $i deve essere una directory esistente
            exit 2
    fi
    
done

TOT=0

for d in $*
do
    for i in $d/*  # modo CORRETTO per ciclare su tutti i file del direttori d
    do
        if [ -f $i ]
        then
            nocc=`cat $i |grep -o $S |  wc -w` 
            echo il file $i nella directory $d contiene $nocc occorrenze di $S #non necessario (verifica)
            if [ $nocc -eq $M ]
            then 
                TOT=`expr $TOT + 1 ` 
                echo $d/$i contiene $M occorrenze di $S
            fi
        fi
    done
done
echo Numero totale di file con $M occorrenze della stringa $S: $TOT

