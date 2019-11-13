#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// store element of logqueue
struct logqueue{
	int head;
	int tail;
	int size;
	int *logqueue;
	unsigned length;

	pthread_mutex_t logQueue;
	pthread_cond_t full;
	pthread_cond_t empty;
};
//create log queue
struct logqueue *createLog(unsigned length){
	struct logqueue* log = (struct logqueue*)malloc(sizeof(struct logqueue));

	log->head = 0;
	log->tail = length - 1;
	log->length = 0;
	log->logqueue = (int *)malloc(log->length * sizeof(int));
	log->size = length;

	pthread_mutex_init(&log->logQueue, NULL);
	pthread_cond_init(&log->full, NULL);
	pthread_cond_init(&log->empty,NULL);
	return log;
}
