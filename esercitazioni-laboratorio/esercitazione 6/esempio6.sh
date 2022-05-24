#!/bin/bash

if [ $# -ne 1 ] ; then echo Sintassi! ; exit; fi #controllo argomenti -> test per vedere se è uno solo

if [  -d $1 ]; then echo $1 è una directory esistente #controllo per vedere se il primo argomento è una directory
else  echo $1 non è un dir!; exit; fi

echo 0 > loop.$$.tmp #crea un file di nome loop.<identificativo>.tmp con su scritto zero -> per scrivere il numero di file nella directory

OK=0

while [ $OK -lt 10 ] #non so cosa faccia -> ciclo for(OK=0; OK<10; OK++)?

do

	new=`ls "$1"|wc -w` #ls visualizza directory | wc -c conta numero di file

	old=`cat loop.$$.tmp` #copia il file new dentro ad old

	if [ $new -ne $old ] #test per vedere se il numero di file è cambiato -> confronto tra old e new -> condizione "sono diversi"

	then #se sono diversi (test = 0 = vero)

	  echo $new > loop.$$.tmp #scrivo il nuovo valore di new sul file -> con id corrente

	  echo in $1 ci sono $new file #stampo il numero di file attuali -> nel caso di cambiamento

	fi

  OK=`expr $OK + 1` #aumento OK di 1 

	sleep 5s #si ferma per 5 secondi -> intervallo di 5 secondi richiesti dalla consegna

done #fine ciclo

rm loop.$$.tmp #elimina file creato
