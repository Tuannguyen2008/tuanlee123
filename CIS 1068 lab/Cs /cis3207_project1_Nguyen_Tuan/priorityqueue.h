#ifndef PRIORITYQUEUE_H

#include<stdio.h>
#include<stdlib.h>
// C code to implement Priority Queue
// using Linked List
#include <stdio.h>
#include <stdlib.h>
#include "otherFunction.h"
 
typedef struct node {
	int time;
    int eventID;
    int jobID;
 
    struct node* next;
 
} Node;
 
Node* newNode(int time,int eventID, int jobID);
Node peek(Node** head);

Node* pop(Node** head);
void push(Node** head, int time, int eventID, int jobID);
int isEmpty(Node* head);
void print_pq( Node* head);

#endif /** PRIORITYQUEUE_H */
