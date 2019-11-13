#ifndef	WORKQUEUE_H
#define WORKQUEUE_H

#include <pthread.h>
#include "logqueue.h"

struct workqueue{
	int max_length;
	int front;
	int rear;
	int *queue;
	unsigned size;

	pthread_mutex_t workQueue;
	pthread_cond_t full;
	pthread_cond_t empty;

};
struct workqueue* createQueue(unsigned size);


void insertqueue(struct workqueue *s, int n);
int removequeue(struct workqueue *s);

struct thread{
	struct workqueue *work;
	struct logqueue *log;
	char **word;
};

#endif