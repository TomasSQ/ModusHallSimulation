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
	int heathens_id[MAX_HEATHENS];
	int prudes_id[MAX_PRUDES];
	int i;


	status = NEUTRAL;
	crossingState = 0;
	crossingPosition = 0;
	prudes = 0;
	heathens = 0;
	inicializarSemaforos();

	printf("inicializando Heathens e Prudes...\n");
	for (i = 0; i < MAX_HEATHENS; i++)
		heathens_id[i] = i + 1;
	heathens_t = newThreads(heathens_id, heathens_f, MAX_HEATHENS);

	for (i = 0; i < MAX_PRUDES; i++)
		prudes_id[i] = (i + 1) * 10;
	prudes_t = newThreads(prudes_id, prudes_f, MAX_PRUDES);

	startThreads(heathens_t);
	startThreads(prudes_t);
	printf("Heathens e Prudes inicializados!\n");

	destruirSemaforos();

	return 0;
}

