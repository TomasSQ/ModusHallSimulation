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

/*
* Retorna o nome do status, normalizado para 22 chars (precisamos normalizar
* para que a animação funciona corretamente, aka workaround)
*/
char* statusStr(int status) {
	switch (status) {
		case NEUTRAL : 
			return "NEUTRAL               ";
		case HEATHENS_RULE :
			return "HEATHENS RULE         ";
		case PRUDES_RULE :
			return "PRUDES RULE           ";
		case TRANSITION_TO_HEATHENS :
			return "TRANSITION TO HEATHENS";
		case TRANSITION_TO_PRUDES :
			return "TRANSITION TO PRUDES  ";
		default :
			return "WTF IS GOING ON?";
	}
}

void renderState (int threadId) {
	int i;
	int heathensQueueSize = heathens;
	int prudesQueueSize = prudes;

	sem_wait(&animation);

	printf("{%02d} ", threadId);

	if (crossingState == HEATHENS_CROSSING) {
		heathensQueueSize--;
	} else if (crossingState == PRUDES_CROSSING) {
		prudesQueueSize--;
	}

	for (i = 0; i < 10; i++) {
		if (i > 9 - heathensQueueSize) {
			printf("[H]");
		} else {
			printf("   ");
		}
	}

	printf(" | ");

	for (i = 0; i < HALL_DISTANCE; i++) {
		if (crossingState != NONE_CROSSING && crossingPosition == i) {
			switch (crossingState) {
				case HEATHENS_CROSSING:
					printf("[H]");
					break;
				case PRUDES_CROSSING:
					printf("[P]");
					break;
				default:
					printf("[who am I?]");
			}
		} else {
			printf("___");
		}
	}

	printf(" | ");

	for (i = 0; i < 10; i++) {
		if (i < prudesQueueSize) {
			printf("[P]");
		} else {
			printf("   ");
		}
	}

	/* Para que os frames da animação não sejam apagados, substitua \r por \n */
	printf("\t%s\r", statusStr(status));
	fflush(stdout);

	sem_post(&animation);
}
