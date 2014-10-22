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

char* stateStr(State state) {
	switch (state) {
		case CRIADA :			return "CRIADA                            ";
		case ESPERANDO_MUTEX :		return "ESPERANDO_MUTEX                   ";
		case ESPERANDO_PRUDES_TURN :	return "ESPERANDO_PRUDES_TURN             ";
		case ESPERANDO_PRUDES_QUEUE :	return "ESPERANDO_PRUDES_QUEUE            ";
		case ESPERANDO_HEATHENS_TURN :	return "ESPERANDO_HEATHENS_TURN           ";
		case ESPERANDO_HEATHENS_QUEUE :	return "ESPERANDO_HEATHENS_QUEUE          ";
		case ESPERANDO_ANIMATION :	return "ESPERANDO_ANIMATION               ";
		case ESPERANDO_CROSSING :	return "ESPERANDO_CROSSING                ";
		case ESPERANDO_DOING_SEMAFORO_POST_MANY : 
						return "ESPERANDO_DOING_SEMAFORO_POST_MANY";
		case EXECUTANDO :		return "EXECUTANDO                        ";
	}

	return "WTF";
}

void renderState (Thread thread) {
	int i;
	int heathensQueueSize = heathens;
	int prudesQueueSize = prudes;

	wait(thread, animation, ESPERANDO_ANIMATION);
	system("clear");

	printf("HEATHENS\t\t\t\t\t\t\tPRUDES\n");
	for (i = 0; i < heathens_t->size; i++) {
		printf("{%02d} %s", heathens_t->threads[i]->id, stateStr(heathens_t->threads[i]->state));
		printf("\t\t\t\t");
		printf("{%02d} %s\n", prudes_t->threads[i]->id, stateStr(prudes_t->threads[i]->state));
	}

	printf("{%02d} ", thread->id);
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
	printf("\t%s", statusStr(status));
	fflush(stdout);

	sem_post(&animation);
}

Thread newThread(int id, void *(*start_routine)(void*)) {
	Thread thread = (Thread) malloc(sizeof(thread));
	thread->id = id;
	thread->state = CRIADA;
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

void wait(Thread thread, semaforo semaforo, State beforeState) {
	thread->state = beforeState;
	sem_wait(&semaforo);
	thread->state = EXECUTANDO;
}

