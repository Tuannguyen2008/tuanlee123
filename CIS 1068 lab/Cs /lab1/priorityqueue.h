#ifndef PRIORITYQUEUE1_H

#include<stdio.h>
#include<stdlib.h>

typedef struct event{
	int time;
	int jobNo;
	int type;
}event;

typedef struct node{
	struct event job;
	struct node *next;

}node;

// Priority queue
void p_append(node** head_ref, event data, int* total);
event p_removeJob(node** head_ref, int* total);

// fifo queue
void append(node** head_ref, event data, int* total);
event removeJob(node** head_ref, int* total);

event p_removeJob(node** head_ref, int* total);
event createEvent(int time, int jobNo, int type);
int randomJob(int min, int max);
void recordEvent(char *line);

#endif /** PRIORITYQUEUE1_H */
