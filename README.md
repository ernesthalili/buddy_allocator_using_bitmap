BUDDY_ALLOCATOR_USING_BITMAP

-Progetto del corso di Sistemi Operativi (Universita La Sapienza) ,tenuto dal professor Giorgio Grisetti

La base del funzionamento della tecnica di BitMap consiste nell'associare ad ogni blocco di memoria, un bit del BitMap.        
Questo bit ha valore 0 se il blocco corrispondente ad esso e' FREE, mentre ha valore 1 se il blocco e' ALLOCATED.             
Per avere un idea piu chiara dello stato di BitMap, ho fornito un albero che mostra per ogni livello i blocchi liberi e occupati.            
Per esempio allocando in totale 1 MB di memoria, avremmo lo seguente stato di BitMap:      
     *livello 0 : [0]    -> blocco di dimensione  1024 KB      
     *livello 1 : [1,2]  -> blocchi di dimensione 1024/2 = 512 KB      
     *livello 2 : [3,4,5,6] -> blocchi di dimensione 512/2 = 256 KB       
     *livello 3 : [7,8,9,10,11,12,13,14]  -> blocchi di dimensione 256/2 = 128 KB       
     .......                        

Per capire il livello in cui il blocco si trova ho usato le formule visto durante il corso: livello=floor(log_2(node_id))
e per trovare l'indice del padre: genitore_id = node_id / 2 

Per facilitare la compilazione ho creato un makefile con i seguenti commandi:  
      * La versione con '_vg' usa valgrind e crea un file su cui si possono vedere i risultati 
  
make main / main_vg -> Un eseguzione che testa il corretto funzionamento del codice 

make clean_main / clean_main_vg -> Cancella tutti i file creati dalla precedente compilazione

make test / test_vg -> Un esecuzione di prova per vedere come funziona l'assgenazione (Allocated / Free) degli indici corrispondenti ai blocchi 

make clean_test / clean_test_vg -> Cancella tutti i file dalla compilazione precedente 


