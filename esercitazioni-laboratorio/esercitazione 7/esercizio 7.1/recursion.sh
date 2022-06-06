#!/bin/bash
# file comandi ricorsivo dell'esercizio 7.1 (AK)
cd "$3"
for file in * ; do    #per ogni elemento contenuto nella directory corrente:
    if test -f "$file" -a -r "$file" #$file è un file ordinario leggibile
    then
        utente=`stat --format="%U" "$file"`  #ricavo lo username del proprietario di $file
        if test $utente = $1 
        then 
            echo `pwd`/$file #stampo il nome assoluto del file trovato
            tail -n $2 $file #stampo le ultime R righe del file corrente
        fi
    elif test -d "$file"  #file è una directory
    then
        echo "Invocazione ricorsiva sulla directory `pwd`/$file"
        "$0" "$1" $2 "$file"  #invocazione ricorsiva
    fi
done

