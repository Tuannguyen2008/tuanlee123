#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// store element of logqueue
struct logqueue{
	int head;
	int tail;
	int size;
	char **array;
	unsigned length;
	int max_size;

	pthread_mutex_t logQueue;
	pthread_cond_t full;
	pthread_cond_t empty;
};
//create log queue
struct logqueue *createLog(unsigned length){
	struct logqueue* log = (struct logqueue*)malloc(sizeof(struct logqueue));

	log->head = 0;
	log->tail = length - 1;
	log->size = 0;
	log->array = malloc(sizeof(char *) * length);
	log->max_size = length;

	pthread_mutex_init(&log->logQueue, NULL);
	pthread_cond_init(&log->full, NULL);
	pthread_cond_init(&log->empty,NULL);
	return log;
};

void insertQueue(struct logqueue *s , char *word){
	
	pthread_mutex_lock(&s->logQueue);

	while(s->size == s->max_size){
		pthread_cond_wait(&s->full , &s->logQueue);
	}
	s->tail = (s->tail + 1) % (s->max_size);
	s->array[s->tail] = word;
	s->size++;
	
	pthread_cond_signal(&s->empty);
	pthread_mutex_unlock(&s->logQueue);
	
}
char *removeQueue(struct logqueue *s){

	char *value;
	
	pthread_mutex_lock(&s->logQueue);

	while(s->size == 0){
		pthread_cond_wait(&s->empty , &s->logQueue);
	}
	value = s->array[s->head];
	s->head = (s->head + 1) % (s->max_size);
	s->size--;

	pthread_cond_signal(&s->full);
	pthread_mutex_unlock(&s->logQueue);

	return value;
}

