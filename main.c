#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COMPARTILHADO_POR_THREADS 0

#define NEUTRAL 				0
#define HEATHENS_RULE 			1
#define PRUDES_RULE				2
#define TRANSITION_TO_HEATHENS	3
#define TRANSITION_TO_PRUDES	4

typedef sem_t semaforo;
typedef semaforo* Semaforo;

int heathens = 0;
int prudes = 0;
int status = NEUTRAL;
semaforo mutex;
semaforo heathensTurn;
semaforo prudesTurn;
semaforo heathensQueue;
semaforo prudesQueue;

pthread_t Heathens;
int heathens_id = 92;
pthread_t Prudes;
int prudes_id = 684;

void inicializarSemaforo(Semaforo semaforo, int valorInicial) {
	sem_init(semaforo, COMPARTILHADO_POR_THREADS, valorInicial);
}

void sem_post_many(Semaforo semaforo, int many) {
	while (many-- > 0)
		sem_post(semaforo);
}

void* heathens_f(void *param) {
	heathens++;

	if (status == NEUTRAL) {
		status = HEATHENS_RULE;
		sem_post(&mutex);
	} else if (status == PRUDES_RULE) {
		if (heathens > prudes) {
			status = TRANSITION_TO_HEATHENS;
			sem_wait(&prudesTurn);
		}

		sem_post(&mutex);
		sem_wait(&heathensQueue);
	} else if (status == TRANSITION_TO_HEATHENS) {
		sem_post(&mutex);
		sem_wait(&heathensQueue);
	} else
		sem_post(&mutex);

  /* cross the field */
 
	sem_wait(&mutex);
	heathens--;

	if (heathens == 0) {
		if (status == TRANSITION_TO_PRUDES)
			sem_post(&prudesTurn);
		if (prudes) {
        	sem_post_many(&prudesQueue, prudes);
        	status = PRUDES_RULE;
		} else
        	status = NEUTRAL;
	}

	if (status == HEATHENS_RULE && prudes > heathens) {
		status = TRANSITION_TO_PRUDES;
		sem_wait(&heathensTurn);
	}

	sem_post(&mutex);

	return NULL;
}

void* prudes_f(void *param) {
	prudes++;

	if (status == NEUTRAL) {
		status = PRUDES_RULE;
		sem_post(&mutex);
	} else if (status == HEATHENS_RULE) {
		if (prudes > heathens) {
			status = TRANSITION_TO_PRUDES;
			sem_wait(&heathensTurn);
		}

		sem_post(&mutex);
		sem_wait(&prudesQueue);
	} else if (status == TRANSITION_TO_PRUDES) {
		sem_post(&mutex);
		sem_wait(&prudesQueue);
	} else
		sem_post(&mutex);

  /* cross the field */
 
	sem_wait(&mutex);
	prudes--;

	if (prudes == 0) {
		if (status == TRANSITION_TO_HEATHENS)
			sem_post(&heathensTurn);
		if (heathens) {
        	sem_post_many(&heathensQueue, prudes);
        	status = PRUDES_RULE;
		} else
        	status = NEUTRAL;
	}

	if (status == PRUDES_RULE && heathens > prudes) {
		status = TRANSITION_TO_HEATHENS;
		sem_wait(&prudesTurn);
	}

	sem_post(&mutex);

	return NULL;
}

int main() {
	inicializarSemaforo(&mutex, 1);
	inicializarSemaforo(&heathensTurn, 1);
	inicializarSemaforo(&prudesTurn, 1);
	inicializarSemaforo(&heathensQueue, 0);
	inicializarSemaforo(&prudesQueue, 0);

	pthread_create(&Heathens, NULL, heathens_f, (void*) &heathens_id);
	pthread_create(&Prudes, NULL, prudes_f, (void*) &prudes_id);

	return 0;
}
