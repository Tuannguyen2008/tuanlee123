#ifndef FIFOQUEUE_H
#include <stdio.h>
#include <stdlib.h>

typedef struct ffnode{
    int time;
    int eventID;
    int jobID;
    struct ffnode *next;
}ffNode;

typedef struct queue{
    int size;
    ffNode *head;
    ffNode *tail;
    int state;
}Queue;

ffNode* createffNode (int time, int eventID , int jobID);
Queue* initializeJobQueue();
void Enqueue(Queue *q , int time , int eventID , int jobID);
ffNode* Deque(Queue *q);
void print_ff( Queue *q);
ffNode* top(Queue *q);
#endif


