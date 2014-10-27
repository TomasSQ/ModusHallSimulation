#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "heathens.h"
#include "prudes.h"

#define COMPARTILHADO_POR_THREADS 0

void inicializarSemaforo(Semaforo semaforo, int valorInicial) {
	sem_init(semaforo, COMPARTILHADO_POR_THREADS, valorInicial);
}

void inicializarSemaforos() {
	printf("inicializando semaforos...\n");

	inicializarSemaforo(&crossing, 1);
	inicializarSemaforo(&animation, 1);
	inicializarSemaforo(&mutex, 1);
	inicializarSemaforo(&heathensTurn, 1);
	inicializarSemaforo(&prudesTurn, 1);
	inicializarSemaforo(&heathensQueue, 0);
	inicializarSemaforo(&prudesQueue, 0);

	printf("semaforos inicializados!\n");
}

void destruirSemaforos() {
	printf("destruindo semaforos...\n");

	sem_destroy(&crossing);
	sem_destroy(&animation);
	sem_destroy(&mutex);
	sem_destroy(&heathensTurn);
	sem_destroy(&prudesTurn);
	sem_destroy(&heathensQueue);
	sem_destroy(&prudesQueue);

	printf("semaforos destruidos!\n");
}

int main() {
	pthread_t heathens_t[MAX_HEATHENS];
	pthread_t prudes_t[MAX_PRUDES];
	int heathens_id[MAX_HEATHENS];
	int prudes_id[MAX_PRUDES];
	int i;

	status = NEUTRAL;
	crossingState = 0;
	crossingPosition = 0;
	prudes = 0;
	heathens = 0;
	inicializarSemaforos();

	printf("Inicializando Heathens e Prudes...\n");
	for (i = 0; i < MAX_HEATHENS; i++) {
		heathens_id[i] = i + 1;
		pthread_create(&heathens_t[i], NULL, heathens_f, (void*) &heathens_id[i]);
	}

	for (i = 0; i < MAX_PRUDES; i++) {
		prudes_id[i] = (i + 1) * 10;
		pthread_create(&prudes_t[i], NULL, prudes_f, (void*) &prudes_id[i]);
	}

	for (i = 0; i < MAX_HEATHENS; i++) {
		pthread_join(heathens_t[i], NULL);
	}

	for (i = 0; i < MAX_PRUDES; i++) {
		pthread_join(prudes_t[i], NULL);
	}

	printf("Heathens e Prudes inicializados!\n");

	destruirSemaforos();

	return 0;
}

