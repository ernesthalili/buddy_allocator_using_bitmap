BUDDY_ALLOCATOR_USING_BITMAP

-Progetto del corso di Sistemi Operativi (Universita La Sapienza) ,tenuto dal professor Giorgio Grisetti

Con questa soluzione, ogni volta che il buddy allocator crea i nodi dell'albero, la dimensione del bitmap verra raddopiata.
Se un blocco e' allocato, allora l'indice che corrisponde a questo blocco viene settato a 1.

Per capire il livello in cui il blocco si trova ho usato le formule visto durante il corso: livello=floor(log_2(node_id))
e per trovare l'indice del padre: genitore_id = node_id / 2 

Per facilitare la compilazione ho creato un makefile con i seguenti commandi:  
      * La versione con '_vg' usa valgrind e crea un file su cui si possono vedere i risultati 
  
make main / main_vg -> Un eseguzione che testa il corretto funzionamento del codice 

make clean_main / clean_main_vg -> Cancella tutti i file creati dalla precedente compilazione

make test / test_vg -> Un esecuzione di prova per vedere come funziona l'assgenazione (Allocated / Free) degli indici corrispondenti ai blocchi 

make clean_test / clean_test_vg -> Cancella tutti i file dalla compilazione precedente 


