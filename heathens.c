#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "heathens.h"

void* heathens_f(void *param) {
	Thread thread = (Thread) param;
	int i;

	thread->state = EXECUTANDO;
	while (1) {
		wait(thread, heathensTurn, ESPERANDO_HEATHENS_TURN);
		sem_post(&heathensTurn);
		wait(thread, mutex, ESPERANDO_MUTEX);
		heathens++;


		if (status == NEUTRAL) {
			status = HEATHENS_RULE;
			sem_post(&mutex);
		} else if (status == PRUDES_RULE) {
			if (heathens > prudes) {
				status = TRANSITION_TO_HEATHENS;
				wait(thread, prudesTurn, ESPERANDO_PRUDES_TURN);
			}

			sem_post(&mutex);
			wait(thread, heathensQueue, ESPERANDO_HEATHENS_QUEUE);
		} else if (status == TRANSITION_TO_HEATHENS) {
			sem_post(&mutex);
			wait(thread, heathensQueue, ESPERANDO_HEATHENS_QUEUE);
		} else
			sem_post(&mutex);

		/* cross the field */
		wait(thread, crossing, ESPERANDO_CROSSING);
		crossingState = HEATHENS_CROSSING;
		for (i = 0; i < HALL_DISTANCE; i++) {
			crossingPosition = i;
			sleep(1);
		}
		crossingState = NONE_CROSSING;

		sem_post(&crossing);

		wait(thread, mutex, ESPERANDO_MUTEX);
		heathens--;

		if (heathens == 0) {
			if (status == TRANSITION_TO_PRUDES)
				sem_post(&prudesTurn);
			if (prudes) {
				sem_post_many(&prudesQueue, 1);
				status = PRUDES_RULE;
			} else
				status = NEUTRAL;
		}

		if (status == HEATHENS_RULE && prudes > heathens) {
			status = TRANSITION_TO_PRUDES;
			wait(thread, heathensTurn, ESPERANDO_HEATHENS_TURN);
		}

		sem_post(&mutex);
		sleep(1);
	}

	return NULL;
}
