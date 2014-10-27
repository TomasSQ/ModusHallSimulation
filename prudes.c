#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "prudes.h"

void* prudes_f(void *param) {
	int i;
	int threadId = *(int *) param;

	while (1) {
		printf("\n%02d arrived\n", threadId);
		sem_wait(&prudesTurn);
		sem_post(&prudesTurn);
		sem_wait(&mutex);
		prudes++;
		renderState(threadId);

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
		sem_wait(&crossing);
		crossingState = PRUDES_CROSSING;
		for (i = HALL_DISTANCE - 1; i >= 0; i--) {
			crossingPosition = i;
			renderState(threadId);
			sleep(1);
		}

		crossingState = NONE_CROSSING;

		sem_post(&crossing);

		sem_wait(&mutex);
		prudes--;
		renderState(threadId);

		if (prudes == 0) {
			if (status == TRANSITION_TO_HEATHENS)
				sem_post(&prudesTurn);

			if (heathens) {
				sem_post_many(&heathensQueue, heathens);
				status = HEATHENS_RULE;
			} else
				status = NEUTRAL;
		}

		if (status == PRUDES_RULE && heathens > prudes) {
			status = TRANSITION_TO_HEATHENS;
			sem_wait(&prudesTurn);
		}

		sem_post(&mutex);
	}

	return NULL;
}
