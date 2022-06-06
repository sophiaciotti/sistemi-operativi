#!/bin/bash
# invoker dell'esercizio 7.1 (AK)
#Controllo argomenti
if [[ $# != 3 ]]; then
    echo "Errore: numero di argomenti non corretto" 
    echo  "Usage: $0 U R dir" 
    exit 1
fi

if [[ $2 = *[!0-9]* ]] ; then
    echo "$2 non è un intero positivo" 
    exit 2
fi

case "$3" in
/*) 
    #la directory inizia per /
    #controllo anche che sia davvero una directory esistente  
    if ! [[ -d "$3" ]]; then
        echo "Errore: $3 non esiste o non è una directory" 
        exit 3
    fi
    ;;
*)
    #il nome della directory non inizia per /, quindi non è un path assoluto
    echo "$3 non è una directory assoluta" 
    exit 4
    ;;
esac

U=$1
R=$2
dir=$3
recfile=recursion
#definizione della stringa da invocare per l'esecuzione del file comandi ricorsivo:
case "$0" in
    # il file comandi è stato invocato con un / Path assoluto.
    /*) 
    dir_name=`dirname $0`
    recursive_command="$dir_name"/$recfile
    ;;
    */*)
    # il file comandi è stato invocato con un path relativo.
    dir_name=`dirname $0`
    recursive_command="`pwd`/$dir_name/$recfile"
    ;;
    *)
    #Path né assoluto né relativo, il comando è nel $PATH
    recursive_command=$recfile
    ;;
esac

#innesco la ricorsione
"$recursive_command" "$U" $R "$dir"

