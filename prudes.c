#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "prudes.h"

void* prudes_f(void *param) {
	while (1) {
		printf("[%02d] 12 esperando prudes turn %d \t\t %s\n", *(int *) param, prudes, statusStr(status));
		sem_wait(&prudesTurn);
		sem_post(&prudesTurn);
		printf("[%02d] 15 esperando mutex %d \t\t %s\n", *(int *) param, prudes, statusStr(status));
		sem_wait(&mutex);
		prudes++;
		printf("[%02d] 18 entrou prudes! Temos %d \t\t %s\n", *(int *) param, prudes, statusStr(status));


		if (status == NEUTRAL) {
			status = PRUDES_RULE;
			sem_post(&mutex);
		} else if (status == HEATHENS_RULE) {
			if (prudes > heathens) {
				status = TRANSITION_TO_PRUDES;
				printf("[%02d] 27 esperando heathens turn %d \t\t %s\n", *(int *) param, prudes, statusStr(status));
				sem_wait(&heathensTurn);
			}

			sem_post(&mutex);
			printf("[%02d] 32 esperando prudes queue turn %d \t\t %s\n", *(int *) param, prudes, statusStr(status));
			sem_wait(&prudesQueue);
		} else if (status == TRANSITION_TO_PRUDES) {
			sem_post(&mutex);
			printf("[%02d] 36 esperando prudes queue turn %d \t\t %s\n", *(int *) param, prudes, statusStr(status));
			sem_wait(&prudesQueue);
		} else
			sem_post(&mutex);

		printf("[%02d] 41 atravessando...\n", *(int *) param);

	/* cross the field */

		sem_wait(&mutex);
		prudes--;
		printf("[%02d] 47 saiu prudes! Temos %d \t\t %s\n", *(int *) param, prudes, statusStr(status));

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
			printf("[%02d] 61 esperando prudes turn %d \t\t %s\n", *(int *) param, prudes, statusStr(status));
			sem_wait(&prudesTurn);
		}

		sem_post(&mutex);
		sleep(1);
	}

	return NULL;
}
