#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "heathens.h"

void* heathens_f(void *param) {
	while (1) {
		printf("[%02d] 12 esperando heathens turn %d \t\t%s\n", *(int *) param, heathens, statusStr(status));
		sem_wait(&heathensTurn);
		sem_post(&heathensTurn);
		printf("[%02d] 15 esperando mutex %d \t\t%s\n", *(int *) param, heathens, statusStr(status));
		sem_wait(&mutex);
		heathens++;
		printf("[%02d] 18 entrou heathens! Temos %d \t\t%s\n", *(int *) param, heathens, statusStr(status));

		if (status == NEUTRAL) {
			status = HEATHENS_RULE;
			sem_post(&mutex);
		} else if (status == PRUDES_RULE) {
			if (heathens > prudes) {
				status = TRANSITION_TO_HEATHENS;
				printf("[%02d] 26 esperando prudes turn %d \t\t%s\n", *(int *) param, heathens, statusStr(status));
				sem_wait(&prudesTurn);
			}


			sem_post(&mutex);
			printf("[%02d] 31 esperando heathens queue turn %d \t\t%s\n", *(int *) param, heathens, statusStr(status));
			sem_wait(&heathensQueue);
		} else if (status == TRANSITION_TO_HEATHENS) {
			sem_post(&mutex);
			printf("[%02d] 35 esperando heathens queue %d \t\t%s\n", *(int *) param, heathens, statusStr(status));
			sem_wait(&heathensQueue);
		} else
			sem_post(&mutex);

		printf("[%02d] 40 atravessando...\n", *(int *) param);

	  /* cross the field */
	 
		sem_wait(&mutex);
		heathens--;
		printf("[%02d] saiu heathens! Temos %d \t\t%s\n", *(int *) param, heathens, statusStr(status));


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
			printf("[%02d] 61 esperando heathens turn %d \t\t %s\n", *(int *) param, prudes, statusStr(status));
			sem_wait(&heathensTurn);
		}

		sem_post(&mutex);
		sleep(1);
	}

	return NULL;
}
