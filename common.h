#ifndef COMMON
#define COMMON

/* hall states */
#define NEUTRAL 				0
#define HEATHENS_RULE 			1
#define PRUDES_RULE				2
#define TRANSITION_TO_HEATHENS	3
#define TRANSITION_TO_PRUDES	4
#define HALL_DISTANCE			4

#define MAX_HEATHENS 2
#define MAX_PRUDES 2

#define NONE_CROSSING			0
#define HEATHENS_CROSSING 		1
#define PRUDES_CROSSING 		2

typedef sem_t semaforo;
typedef semaforo* Semaforo;

int crossingState;
int crossingPosition;

int status;
semaforo mutex;

int prudes;
semaforo prudesTurn;
semaforo prudesQueue;

int heathens;
semaforo heathensTurn;
semaforo heathensQueue;

semaforo animation;
semaforo crossing;

void sem_post_many(Semaforo semaforo, int many);

char* statusStr(int status);

void renderState (int threadId);

#endif