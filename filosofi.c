#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define NUM_FILOSOFI 5
#define NUM_PASTI 3

/* Ogni mutex rappresenta una forchetta condivisa da due filosofi. */
pthread_mutex_t forchette[NUM_FILOSOFI];

/* Ferma un thread per pochi millisecondi, per simulare il tempo che passa. */
void pausa(long millisecondi)
{
    struct timespec tempo;

    tempo.tv_sec = millisecondi / 1000;
    tempo.tv_nsec = (millisecondi % 1000) * 1000000L;
    nanosleep(&tempo, NULL);
}

/* Questa funzione viene eseguita da ogni thread-filosofo. */
void *filosofo(void *argomento)
{
    int id = *(int *)argomento;
    int sinistra = id;
    int destra = (id + 1) % NUM_FILOSOFI;
    int prima;
    int seconda;
    int pasto;

    /*
     * Tutti prendono prima la forchetta con il numero più basso.
     * Così non si crea un'attesa circolare e non avviene il deadlock.
     */
    if (sinistra < destra) {
        prima = sinistra;
        seconda = destra;
    } else {
        prima = destra;
        seconda = sinistra;
    }

    for (pasto = 1; pasto <= NUM_PASTI; pasto++) {
        printf("Filosofo %d sta pensando.\n", id + 1);
        pausa(100 + id * 30);

        printf("Filosofo %d ha fame.\n", id + 1);

        /* Se una forchetta è occupata, il thread aspetta qui. */
        pthread_mutex_lock(&forchette[prima]);
        pthread_mutex_lock(&forchette[seconda]);

        printf("Filosofo %d sta mangiando (pasto %d/%d).\n",
               id + 1, pasto, NUM_PASTI);
        pausa(150 + id * 30);

        pthread_mutex_unlock(&forchette[seconda]);
        pthread_mutex_unlock(&forchette[prima]);
    }

    printf("Filosofo %d ha terminato.\n", id + 1);
    return NULL;
}

int main(void)
{
    pthread_t thread[NUM_FILOSOFI];
    int id[NUM_FILOSOFI];
    int i;

    /* Prepariamo le cinque forchette. */
    for (i = 0; i < NUM_FILOSOFI; i++) {
        pthread_mutex_init(&forchette[i], NULL);
    }

    /* Creiamo un thread per ogni filosofo. */
    for (i = 0; i < NUM_FILOSOFI; i++) {
        id[i] = i;
        pthread_create(&thread[i], NULL, filosofo, &id[i]);
    }

    /* Il main aspetta che tutti i filosofi abbiano finito. */
    for (i = 0; i < NUM_FILOSOFI; i++) {
        pthread_join(thread[i], NULL);
    }

    /* Le forchette non servono più e possono essere distrutte. */
    for (i = 0; i < NUM_FILOSOFI; i++) {
        pthread_mutex_destroy(&forchette[i]);
    }

    printf("\nTutti i filosofi hanno terminato senza deadlock.\n");
    return 0;
}
