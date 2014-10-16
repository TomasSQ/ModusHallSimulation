#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "heathens.h"

void* heathens_f(void *param) {
	while (heathens < MAX_HEATHENS) {
		printf("heathens\n");
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
		sleep(1);
	}

	return NULL;
}

