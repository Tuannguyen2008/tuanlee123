#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//store workqueue
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
//create work queue
struct workqueue* createQueue(unsigned size){ 
	struct workqueue* que = (struct workqueue*) malloc(sizeof(struct workqueue));
	
	que->length = size;
	que->front = 0;
	que->size = 0;
	que->rear = size - 1;
	que->queue = (int *)malloc(que->size * sizeof(int));

	pthread_mutex_init(&que->workQueue, NULL);
	pthread_cond_init(&que->full, NULL);
	pthread_cond_init(&que->empty,NULL);
	return que;
}

/*
//add new queue
void insertqueue(workqueue *s, int n ){
	struct Node *tmp = newnode(n);
	
	if(s->rear == NULL){ // queue is empty
		s.rear->next = tmp; 
	}
	else{		// add new node 
		q->rear->next = tmp;
		q->rear = tmp;
	}
}

//remove queue and return it value
struct Node *removequeue(workqueue *s){
	queueNode *newNode;
	int value;
	value = s->front->total;
	newNode = s->front;
	s->front = s->front->next;
	s->length--;

	return value;
}  */
