#!/bin/bash

# esercitazione6.3.sh 

if test $# -ne 3
then echo Errore nel numero di parametri
        exit
fi
nomefile=$1
utente=$2
filedir=$3
if test -f $filedir -a -r $filedir #è un file AND è leggibile
then 
    echo ok
else
    echo il file $filedir non può essere letto
    exit
fi

for d in `cat $filedir` #ripete per ogni parola contenuta nel file
do
    if ! test -d $d #se non è una directory esistente 
    then    echo il nome $d non corrisponde a  una directory esistente
    else #se è una directory esistente 
        dacercare=$d/$nomefile  #nome assoluto del file da cercare
        if test -f $dacercare # se il file esiste
        then 
            owner=`ls -l $dacercare | cut -d ' ' -f3` #estrazione username 
            if test $owner = $utente 
            then  
                dim=`cat $dacercare | wc -c` 
                echo Il file $nomefile dell’utente $utente nella directory $d contiene $dim caratteri.
            fi
        fi
        
    fi
done
