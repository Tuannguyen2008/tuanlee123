#ifndef	LOGQUEUE_H
#define LOGQUEUE_H

#include <pthread.h>

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
struct logqueue *createLog(unsigned length);

void insertQueue(struct logqueue *s , char *word);
char *removeQueue(struct logqueue *s);

#endif