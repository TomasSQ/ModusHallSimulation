#ifndef COMMON
#define COMMON

#define NEUTRAL 				0
#define HEATHENS_RULE 			1
#define PRUDES_RULE				2
#define TRANSITION_TO_HEATHENS	3
#define TRANSITION_TO_PRUDES	4

typedef sem_t semaforo;
typedef semaforo* Semaforo;

int crossing;
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

void sem_post_many(Semaforo semaforo, int many);

char* statusStr(int status);

void renderState ();

#endif