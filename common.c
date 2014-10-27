#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"

char* legs1 = " |\\";
char* legs2 = "/ \\";
char* legs3 = "/| ";
char* legs4 = " ||";
char* legs5 = "|| ";

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
		case NEUTRAL : return "NEUTRAL               ";
		case HEATHENS_RULE : return "HEATHENS RULE         ";
		case PRUDES_RULE : return "PRUDES RULE           ";
		case TRANSITION_TO_HEATHENS : return "TRANSITION TO HEATHENS";
		case TRANSITION_TO_PRUDES : return "TRANSITION TO PRUDES  ";
		default : return "WTF IS GOING ON?";
	}
}

void renderState (int threadId) {
	int i;
	int heathensQueueSize = heathens;
	int heathensQueueStartPosition = MAX_HEATHENS - heathensQueueSize;
	int prudesQueueSize = prudes;
	/*int mutexInt, prudesTurnInt, heathensTurnInt, prudesQueueInt, heathensQueueInt;*/

	sem_wait(&animation);

	system("clear");

	for (i = 0; i < 43; i++) printf(" ");
	printf("Modus Hall\n\n");

	printf("Thread ativa:\t%02d\n", threadId);
	printf("Estado:\t\t%s\n", statusStr(status));

	printf("Prudes:\t\t%02d\n", prudes);
	printf("Heathens:\t%02d\n", heathens);

	printf("\n\n");

	if (crossingState == 1) {
		heathensQueueSize--;
	} else if (crossingState == 2) {
		prudesQueueSize--;
	}

	/* Field top border */
	for (i = 0; i < 3 * MAX_HEATHENS; i++) printf(" ");
	printf("   ");

	for (i = 0; i < (3 * HALL_DISTANCE); i++) printf("_");
	printf("\n");

	/* Head animation */
	for (i = 0; i < MAX_HEATHENS; i++) {
		if (i > heathensQueueStartPosition) {
			printf("_O_");
		} else {
			printf("   ");
		}
	}

	printf("   ");

	for (i = 0; i < HALL_DISTANCE; i++) {
		if (crossingState != 0 && crossingPosition == i) {
			printf(" O ");
		} else {
			printf("   ");
		}
	}

	printf("   ");

	for (i = 0; i < MAX_PRUDES; i++) {
		if (i < prudesQueueSize) {
			printf("_O_");
		} else {
			printf("   ");
		}
	}

	printf("\n");

	/* Torso animation */
	for (i = 0; i < MAX_HEATHENS; i++) {
		if (i > heathensQueueStartPosition) {
			printf(" | ");
		} else {
			printf("   ");
		}
	}

	printf("   ");

	for (i = 0; i < HALL_DISTANCE; i++) {
		if (crossingState != 0 && crossingPosition == i) {
			printf("/|\\");
		} else {
			printf("   ");
		}
	}

	printf("   ");

	for (i = 0; i < MAX_PRUDES; i++) {
		if (i < prudesQueueSize) {
			printf(" | ");
		} else {
			printf("   ");
		}
	}

	printf("\n");

	/* Legs animation */
	for (i = 0; i < MAX_HEATHENS; i++) {
		if (i > heathensQueueStartPosition) {
			printf("/ \\");
		} else {
			printf("   ");
		}
	}

	printf("   ");

	for (i = 0; i < HALL_DISTANCE; i++) {
		if (crossingState != 0 && crossingPosition == i) {
			switch (crossingPosition % 3) {
				case 0:
					printf("%s", (crossingState == 1 ? legs1 : legs3));
					break;
				case 1:
					printf("%s", legs2);
					break;
				case 2:
					printf("%s", (crossingState == 1 ? legs4 : legs5));
					break;
			}
		} else {
			printf("   ");
		}
	}

	printf("   ");

	for (i = 0; i < MAX_PRUDES; i++) {
		if (i < prudesQueueSize) {
			printf("/ \\");
		} else {
			printf("   ");
		}
	}

	printf("\n");

	/* Field bottom border */
	for (i = 0; i < 3 * MAX_HEATHENS; i++) printf(" ");
	printf("   ");

	for (i = 0; i < (3 * HALL_DISTANCE); i++) printf("_");
	printf("\n");

	for (i = 0; i < MAX_HEATHENS; i++) {
		if (i > heathensQueueStartPosition) {
			printf("[H]");
		} else {
			printf("   ");
		}
	}

	printf(" | ");

	for (i = 0; i < HALL_DISTANCE; i++) {
		if (crossingState != 0 && crossingPosition == i) {
			if (crossingState == 1) {
				printf("[H]");
			} else if (crossingState == 2) {
				printf("[P]");
			} else {
				printf("[who am I?]");
			}
		} else {
			printf("   ");
		}
	}

	printf(" | ");

	for (i = 0; i < MAX_PRUDES; i++) {
		if (i < prudesQueueSize) {
			printf("[P]");
		} else {
			printf("   ");
		}
	}
	
	/* Debug
	sem_getvalue(&mutex, &mutexInt);
	sem_getvalue(&prudesTurn, &prudesTurnInt);
	sem_getvalue(&heathensTurn, &heathensTurnInt);
	sem_getvalue(&prudesQueue, &prudesQueueInt);
	sem_getvalue(&heathensQueue, &heathensQueueInt);

	printf("mutex: %d, prudesTurn: %d, heathensTurn: %d, prudesQueue: %d, heathensQueue: %d\n", mutexInt, prudesTurnInt, heathensTurnInt, prudesQueueInt, heathensQueueInt); */
	fflush(stdout);

	sem_post(&animation);
}
