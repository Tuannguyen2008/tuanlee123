#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Node
typedef struct ffnode{
    int time;
    int eventID;
    int jobID;
    struct ffnode *next;
} ffNode;

//Queue
typedef struct queue{
    int size;
    ffNode *head;
    ffNode *tail;
    int state;
} Queue;

//create new node
ffNode* createffNode (int time, int eventID , int jobID){
    ffNode* newnode = (ffNode*)malloc(sizeof(ffNode));
    if(newnode == NULL){
        printf("Null pointers");
    }

    newnode->time = time;
    newnode->eventID = eventID;
    newnode->jobID = jobID;
    newnode->next = NULL;
    return newnode;
}


//initializeQueue
Queue* initializeJobQueue(){
    Queue* q =(Queue*)malloc(sizeof(Queue));
    q->head = q->tail = NULL;
    q->size = 0;
    q->state = 0;//not busy
    return q;
}
//Enqueue

void Enqueue(Queue *q , int time , int eventID, int jobID){
    ffNode *node = createffNode(time, eventID , jobID);
    if(q->head == NULL && q->tail == NULL){
        q->head = q->tail = node;
    }
    else{
        q->tail->next = node;
        q->tail = node;
    }
    q->size++;



}

//Deque
ffNode* Deque(Queue *q){
    ffNode* deleted = q->head;
    ffNode* cur = createffNode(q->head->time, q->head->eventID, q->head->jobID);
    if(q->head == NULL) return NULL;
    else if(q->head == q->tail)
      q->head = q->tail = NULL;
    else{
        q->head = q->head->next;
    }

    free(deleted);
    q->size--;
    return cur;

}


void print_ff(Queue* q)
{
  ffNode* cur = q->head;
  printf("Fifo Queue:\n");
  if(cur == NULL)
    {
      printf( "empty ll" );
    }
  while(cur != NULL)
    {
      printf( "JobID: %d " , cur->jobID);
      printf( "EventID: %d " , cur->eventID);
      printf( "Time: %d \n", cur->time);
      cur = cur->next;

    }
  printf( "\n" );
}

//get top of the queue
ffNode* top(Queue *q){
  return q->head;
}

