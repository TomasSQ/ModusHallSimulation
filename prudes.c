#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "prudes.h"

void* prudes_f(void *param) {
	Thread thread = (Thread) param;
	int i;

	while (1) {
		wait(thread, prudesTurn, ESPERANDO_PRUDES_TURN);
		sem_post(&prudesTurn);
		wait(thread, mutex, ESPERANDO_MUTEX);
		prudes++;

		if (status == NEUTRAL) {
			status = PRUDES_RULE;

			sem_post(&mutex);
		} else if (status == HEATHENS_RULE) {
			if (prudes > heathens) {
				status = TRANSITION_TO_PRUDES;

				wait(thread, heathensTurn, ESPERANDO_HEATHENS_TURN);
			}

			sem_post(&mutex);
			wait(thread, prudesQueue, ESPERANDO_PRUDES_QUEUE);
		} else if (status == TRANSITION_TO_PRUDES) {
			sem_post(&mutex);
			wait(thread, prudesQueue, ESPERANDO_PRUDES_QUEUE);
		} else
			sem_post(&mutex);

		/* cross the field */
		wait(thread, crossing, ESPERANDO_CROSSING);
		crossingState = PRUDES_CROSSING;
		for (i = HALL_DISTANCE - 1; i >= 0; i--) {
			crossingPosition = i;
			sleep(1);
		}

		crossingState = NONE_CROSSING;

		sem_post(&crossing);

		wait(thread, mutex, ESPERANDO_MUTEX);
		prudes--;

		if (prudes == 0) {
			if (status == TRANSITION_TO_HEATHENS)
				sem_post(&heathensTurn);
			if (heathens) {
				sem_post_many(&heathensQueue, 1);
				status = HEATHENS_RULE;
			} else
				status = NEUTRAL;
		}

		if (status == PRUDES_RULE && heathens > prudes) {
			status = TRANSITION_TO_HEATHENS;
			wait(thread, prudesTurn, ESPERANDO_PRUDES_TURN);
		}

		sem_post(&mutex);
		sleep(1);
	}

	return NULL;
}
