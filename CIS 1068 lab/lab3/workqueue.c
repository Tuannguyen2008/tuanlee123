#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "logqueue.h"

//store workqueue
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
//create work queue
struct workqueue* createQueue(unsigned size){ 
	struct workqueue* que = (struct workqueue*) malloc(sizeof(struct workqueue));
	
	que->max_length = size;
	que->front = 0;
	que->size = 0;
	que->rear = size - 1;
	que->queue = (int *)malloc(que->size * sizeof(int));

	pthread_mutex_init(&que->workQueue, NULL);
	pthread_cond_init(&que->full, NULL);
	pthread_cond_init(&que->empty,NULL);
	return que;
};


//add new queue
void insertqueue(struct workqueue *s, int n ){
	
	pthread_mutex_lock(&s->workQueue);

	while(s->size == s->max_length){
		pthread_cond_wait(&s->full , &s->workQueue);
	}
	s->rear = (s->rear + 1) % (s->max_length);
	s->queue[s->rear] = n;
	s->size++;
	
	pthread_cond_signal(&s->empty);
	pthread_mutex_unlock(&s->workQueue);
}

//remove queue and return it value
int removequeue(struct workqueue *s){
	int value;
	
	pthread_mutex_lock(&s->workQueue);

	while(s->size == 0){
		pthread_cond_wait(&s->empty , &s->workQueue);
	}
	value = s->queue[s->front];
	s->front = (s->front + 1) % (s->max_length);
	s->size--;
	
	pthread_cond_signal(&s->full);
	pthread_mutex_unlock(&s->workQueue);

	return value;
}


struct thread{
	struct workqueue *work;
	struct logqueue *log;
	char **word;
};
