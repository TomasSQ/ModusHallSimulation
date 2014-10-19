#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"

void sem_post_many(Semaforo semaforo, int many) {
	while (many-- > 0)
		sem_post(semaforo);
}

char* statusStr(int status) {
	switch (status) {
		case NEUTRAL : return "NEUTRAL";
		case HEATHENS_RULE : return "HEATHENS RULE";
		case PRUDES_RULE : return "PRUDES_RULE";
		case TRANSITION_TO_HEATHENS : return "TRANSITION TO HEATHENS";
		case TRANSITION_TO_PRUDES : return "TRANSITION TO PRUDES";
		default : return "PIROCA";
	}
}

void renderState (int threadId) {
	int i;
	int heathensQueueSize = heathens;
	int prudesQueueSize = prudes;

	sem_wait(&animation);

	printf("{%02d} ", threadId);

	if (crossingState == 1) {
		heathensQueueSize--;
	} else if (crossingState == 2) {
		prudesQueueSize--;
	}

	for (i = 0; i < 10; i++) {
		if (i > (9 - heathensQueueSize)) {
			printf("[H]");
		} else {
			printf("   ");
		}
	}

	for (i = 0; i < 10; i++) {
		if (crossingState != 0 && crossingPosition == i) {
			if (crossingState == 1) {
				printf("[H]");
			} else if (crossingState == 2) {
				printf("[P]");
			} else {
				printf("[who am I?]");
			}
		} else {
			printf("___");
		}
	}

	for (i = 0; i < 10; i++) {
		if (i < prudesQueueSize) {
			printf("[P]");
		} else {
			printf("   ");
		}
	}

	printf("\n");

	sem_post(&animation);
}