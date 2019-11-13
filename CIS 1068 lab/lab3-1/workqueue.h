#ifndef	WORKQUEUE_H
#define WORKQUEUE_H

#include <pthread.h>

struct workqueue{
	int length;
	int front;
	int rear;
	int *queue;
	unsigned size;

	pthread_mutex_t workQueue;
	pthread_cond_t full;
	pthread_cond_t empty;

};
struct workqueue* createQueue(unsigned size);

#endif