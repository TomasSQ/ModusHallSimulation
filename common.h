#include <pthread.h>

#ifndef COMMON
#define COMMON

/* hall states */
#define NEUTRAL 				0
#define HEATHENS_RULE 			1
#define PRUDES_RULE				2
#define TRANSITION_TO_HEATHENS	3
#define TRANSITION_TO_PRUDES	4

#define HALL_DISTANCE			10

#define NONE_CROSSING			0
#define HEATHENS_CROSSING 		1
#define PRUDES_CROSSING 		2

typedef sem_t semaforo;
typedef semaforo* Semaforo;

typedef enum State {
	CRIADA,
	ESPERANDO_MUTEX,
	ESPERANDO_PRUDES_TURN,
	ESPERANDO_PRUDES_QUEUE,
	ESPERANDO_HEATHENS_TURN,
	ESPERANDO_HEATHENS_QUEUE,
	ESPERANDO_ANIMATION,
	ESPERANDO_CROSSING,
	ESPERANDO_DOING_SEMAFORO_POST_MANY,
	EXECUTANDO
} State;

typedef struct {
	int id;
	State state;
	pthread_t thread;
} thread;
typedef thread* Thread;

typedef struct {
	int size;
	Thread* threads;
} threads;
typedef threads* Threads;

Threads heathens_t;
Threads prudes_t;

volatile int crossingState;
volatile int crossingPosition;

volatile int status;
semaforo mutex;

volatile int prudes;
semaforo prudesTurn;
semaforo prudesQueue;

volatile int heathens;
semaforo heathensTurn;
semaforo heathensQueue;

semaforo animation;
semaforo crossing;

semaforo doingSemaforoPostMany;

void sem_post_many(Semaforo semaforo, int many);

char* statusStr(int status);

void renderState (Thread threadId);

Thread newThread(int id, void *(*start_routine)(void*));
Threads newThreads(int* ids, void *(*start_routine)(void*), int size);
void startThreads(Threads threads);
void wait(Thread thread, semaforo semaforo, State beforeState);

#endif
