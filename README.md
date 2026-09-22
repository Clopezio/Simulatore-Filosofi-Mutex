# Simulatore dei cinque filosofi con mutex

Questo progetto mostra una soluzione semplice al problema dei cinque filosofi.
È scritto in C e usa i thread POSIX (`pthread`) e un mutex per ogni forchetta.

## Come eseguirlo

```sh
make
./filosofi
```

Ogni filosofo esegue 3 pasti. Per cambiare questo numero basta modificare
`NUM_PASTI` all'inizio di `filosofi.c`.

Per compilare ed eseguire direttamente:

```sh
make run
```

## Come funziona

I cinque filosofi sono rappresentati da cinque thread. Tra ogni coppia di
filosofi c'è una forchetta, rappresentata da un `pthread_mutex_t`. Un filosofo
può quindi mangiare solo dopo aver bloccato entrambi i mutex delle sue
forchette. Finito il pasto, li sblocca subito.

La soluzione evita il deadlock imponendo a tutti la stessa regola: prendere
prima la forchetta con il numero più basso e poi quella con il numero più alto.
In questo modo non può formarsi una catena circolare in cui ogni filosofo tiene
una forchetta e aspetta indefinitamente la successiva.

La simulazione ha una durata finita. Il `main` aspetta tutti i thread con
`pthread_join` e distrugge i mutex prima di terminare. Il programma usa solo
array, variabili e puntatori semplici.

## Nota sulla correttezza

L'ordine globale delle forchette garantisce l'assenza di deadlock. Come nelle
normali implementazioni di `pthread_mutex_t`, l'ordine con cui i thread in
attesa ottengono un mutex dipende dallo scheduler del sistema operativo; non è
quindi una dimostrazione generale di assenza di starvation. Poiché ogni
filosofo esegue un numero finito di pasti, tutti i thread della simulazione
possono comunque completare il proprio lavoro.
