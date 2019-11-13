#include<stdio.h>
#include<stdlib.h>
#include"otherFunction.h"

// Node
typedef struct node {
    int time;
    int eventID;
    int jobID;
    struct node* next;
} Node;
 
//Create A New Node
Node* newNode(int time, int eventID , int jobID){
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->eventID = eventID;
    temp->time = time;
    temp->jobID = jobID;
    temp->next = NULL;
    return temp;
}
 
// Return the value at head
Node* peek(Node** head){
    Node* cur = *head;
    return cur;
}
 
// Removes the element with the
// highest time form the list
Node* pop(Node** head){
    Node* temp = *head;
    Node* cur = newNode(temp->time , temp->eventID, temp->jobID);
    if((*head) == NULL) return NULL;
    else if((*head)->next == NULL) (*head) = NULL;
    else{
      (*head) = (*head)->next;
    }
    free(temp);
    
    appendToFile(cur->time , cur->eventID , cur->jobID , "out.txt");
    return cur;
}

//enqueue 
void push(Node** head, int time, int eventID ,  int jobID)
{

    Node* cur = (*head);
    Node* temp = newNode(time,eventID,jobID);
    if ((*head)->time > time){
        temp->next = *head;
        (*head) = temp;
    }
    else{
        while (cur->next != NULL &&
          cur->next->time < time) {
            cur = cur->next;
        }
 
        
        temp->next = cur->next;
        cur->next = temp;
    }

}
 
//check whether list is empty
int isEmpty(Node* head)
{
    if(head == 0){
      return 1;
    } else return 0;
}


//print priority_queue
void print_pq( Node* head )
{
  printf( "Priority Queue:\n"); 
  if( head == 0 )
    {
      printf( "empty ll" );
    }
  while( head != 0 )
    {
      printf( "JobID: %d " , head->jobID);
      printf( "Time: %d ", head->time );
      printf( "EventID: %d \n", head->eventID );
      head = head->next;

    }  
  printf( "\n" );
}

