#ifndef	LOGQUEUE_H
#define LOGQUEUE_H

#include <pthread.h>

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
struct logqueue *createLog(unsigned length);

#endif