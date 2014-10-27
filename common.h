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

#define SLEEP_DELAY				125000

typedef struct {
	int id;
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

typedef sem_t semaforo;
typedef semaforo* Semaforo;

int crossingState;
int crossingPosition;

int status;
semaforo mutex;

int prudes;
int maxPrudes;
semaforo prudesTurn;
semaforo prudesQueue;

int heathens;
int maxHeathens;
semaforo heathensTurn;
semaforo heathensQueue;

semaforo animation;
semaforo crossing;

void sem_post_many(Semaforo semaforo, int many);

char* statusStr(int status);

void renderState (Thread threadId);

Thread newThread(int id, void *(*start_routine)(void*));
Threads newThreads(int* ids, void *(*start_routine)(void*), int size);
void startThreads(Threads threads);

#endif

