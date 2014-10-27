#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"

/* Desenhos para posições da cabeça  na animação */
char* waitingHead = "_O_";
char* walkingHead = " O ";

/* Desenhos para posições do torso na animação */
char* waitingTorso = " | ";
char* walkingTorso = "/|\\";

/* Desenhos para posições das pernas na animação */
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
* Retorna o nome do status.
*/
char* statusStr(int status) {
	switch (status) {
		case NEUTRAL:				return "NEUTRAL";
		case HEATHENS_RULE:			return "HEATHENS RULE";
		case PRUDES_RULE:			return "PRUDES RULE";
		case TRANSITION_TO_HEATHENS:return "TRANSITION TO HEATHENS";
		case TRANSITION_TO_PRUDES:	return "TRANSITION TO PRUDES";
		default :					return "WTF IS GOING ON?";
	}
}

void printNSpaces(int n) {
	int i;
	for (i = 0; i < n; i++)
		printf(" ");
}

void printHall() {
	int i;
	for (i = 0; i < 3 * HALL_DISTANCE; i++)
		printf("_");
}

void printWaitingAndWalking(char* waiting, char *walking, int heathensQueueStartPosition, int prudesQueueSize) {
	int i;
	for (i = 0; i < maxHeathens; i++)
		if (i > heathensQueueStartPosition)
			printf("%s", waiting);
		else
			printf("   ");

	printf("   ");

	for (i = 0; i < HALL_DISTANCE; i++)
		if (crossingState != 0 && crossingPosition == i)
			printf("%s", walking);
		else
			printf("   ");

	printf("   ");

	for (i = 0; i < maxPrudes; i++)
		if (i < prudesQueueSize)
			printf("%s", waiting);
		else
			printf("   ");

	printf("\n");
}

void printLegsAnimation(int heathensQueueStartPosition, int prudesQueueSize) {
	int i;
	for (i = 0; i < maxHeathens; i++)
		if (i > heathensQueueStartPosition)
			printf("%s", legs2);
		else
			printf("   ");

	printf("   ");

	for (i = 0; i < HALL_DISTANCE; i++)
		if (crossingState != 0 && crossingPosition == i)
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
		else
			printf("   ");

	printf("   ");

	for (i = 0; i < maxPrudes; i++)
		if (i < prudesQueueSize)
			printf("%s", legs2);
		else
			printf("   ");

	printf("\n");
}

void renderState (Thread thread) {
	int i;
	int heathensQueueSize = heathens;
	int heathensQueueStartPosition = maxHeathens - heathensQueueSize;
	int prudesQueueSize = prudes;

	sem_wait(&animation);

	system("clear");

	printNSpaces(43);
	printf("Modus Hall\n\n");

	printf("Thread ativa:\t%02d\n", thread->id);
	printf("Estado:\t\t%s\n", statusStr(status));

	printf("Prudes:\t\t%02d\n", prudes);
	printf("Heathens:\t%02d\n", heathens);

	printf("\n\n");

	if (crossingState == HEATHENS_CROSSING)
		heathensQueueSize--;
	else if (crossingState == PRUDES_CROSSING)
		prudesQueueSize--;

	/* Field top border */
	printNSpaces(3 * maxHeathens);
	printf("   ");

	printHall();
	printf("\n");

	/* Head animation */
	printWaitingAndWalking(waitingHead, walkingHead, heathensQueueStartPosition, prudesQueueSize);
	
	/* Torso animation */
	printWaitingAndWalking(waitingTorso, walkingTorso, heathensQueueStartPosition, prudesQueueSize);

	/* Legs animation */
	printLegsAnimation(heathensQueueStartPosition, prudesQueueSize);

	/* Field bottom border */
	printNSpaces(3 * maxHeathens);
	printf("   ");

	printHall();
	printf("\n");

	for (i = 0; i < maxHeathens; i++)
		if (i > heathensQueueStartPosition)
			printf("[H]");
		else
			printf("   ");

	printf(" | ");

	for (i = 0; i < HALL_DISTANCE; i++)
		if (crossingState != NONE_CROSSING && crossingPosition == i)
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
		else
			printf("   ");

	printf(" | ");

	for (i = 0; i < maxPrudes; i++)
		if (i < prudesQueueSize)
			printf("[P]");
		else
			printf("   ");

	fflush(stdout);

	sem_post(&animation);
}

Thread newThread(int id, void *(*start_routine)(void*)) {
	Thread thread = (Thread) malloc(sizeof(thread));
	thread->id = id;
	pthread_create(&thread->thread, NULL, start_routine, (void*) thread);

	return thread;
}

Threads newThreads(int* ids, void *(*start_routine)(void*), int size) {
	int i;
	Threads threads = (Threads) malloc(sizeof(threads));
	threads->size = size;

	threads->threads = (Thread*) malloc(sizeof(Thread) * size);

	for (i = 0; i < threads->size; i++)
		threads->threads[i] = newThread(ids[i], start_routine);

	return threads;
}

void startThreads(Threads threads) {
	int i;

	for (i = 0; i < threads->size; i++)
		pthread_join(threads->threads[i]->thread, NULL);
}

