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