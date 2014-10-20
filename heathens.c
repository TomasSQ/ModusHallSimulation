#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "heathens.h"

void* heathens_f(void *param) {
	int i;
	int threadId = *(int *) param;

	while (1) {
		sem_wait(&heathensTurn);
		sem_post(&heathensTurn);
		sem_wait(&mutex);
		heathens++;

		renderState(threadId);

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
		sem_wait(&crossing);
		crossingState = 1;
		for (i = 0; i < HALL_DISTANCE; i++) {
			crossingPosition = i;
			renderState(threadId);
			sleep(1);
		}
		crossingState = 0;

		sem_post(&crossing);

		sem_wait(&mutex);
		heathens--;
		renderState(threadId);

		if (heathens == 0) {
			if (status == TRANSITION_TO_PRUDES)
				sem_post(&prudesTurn);
			if (prudes) {
				sem_post(&prudesQueue);
				status = PRUDES_RULE;
			} else
				status = NEUTRAL;
		}

		if (status == HEATHENS_RULE && prudes > heathens) {
			status = TRANSITION_TO_PRUDES;
			sem_wait(&heathensTurn);
		}

		sem_post(&mutex);
		sleep(1);
	}

	return NULL;
}