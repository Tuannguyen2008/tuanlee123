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


//priority queue add job
void p_append(node** head_ref, event data, int* total){
	node* temp = (node*) malloc(sizeof(node));
	temp->job = data;
	temp->next = NULL;

	int priority = temp->job.time;
	//printf("Job number: %d, Job Time: %d\n",temp->job.jobNo,temp->job.time);
	//printf("inserting priority time: %d, job %d\n",priority,temp->job.jobNo);
	(*total)++;
	//start at beginning of list
	node *last = *head_ref;

	if (*head_ref == NULL){
		*head_ref = temp;
		return;
	}else if(priority < last->job.time){
		temp->next = last;
		*head_ref = temp; // have the pointer to the linkedlist point to the very start of the node which would be temp
		return;
	}

	//traverse till we get to the end of list OR if we find the correct position where the node belongs
	while (last->next != NULL && priority >= last->next->job.time)
		last = last->next;

	temp->next = last->next;
	last->next = temp;
	return;
}

//priority queue remove job
event p_removeJob(node** head_ref, int* total){
	node *temp = *head_ref;
	*head_ref = (*head_ref)->next;
	event job = temp->job;
	free(temp);
	(*total)--;
	return job;
}

//create new job
event createEvent(int time, int jobNo, int type){
	event job;
	job.time = time;
	job.jobNo = jobNo;
	job.type = type;
	return job;
}

// create random job
int randomJob(int min, int max){
	return (rand() % (max - min + 1)) + min;
}

// put all value to out.txt file
void recordEvent(char *line){
	FILE * fp;
	/* open the file for writing*/
	fp = fopen ("out.txt","a");
	fprintf (fp,"%s\n",line);

   /* close the file*/
   fclose (fp);
}

//fifo queue add next job
void append(node** head_ref, event data, int* total){
	node* new_node = (node*) malloc(sizeof(node));
	new_node->job = data;
	new_node->next = NULL;

	//start at beginning of list
	node *last = *head_ref;
	(*total)++;
	if (*head_ref == NULL){
		*head_ref = new_node;
		return;
	}
	while (last->next != NULL)
		last = last->next;

	last->next = new_node;
	return;
}

//fifo queue remove job
event removeJob(node** head_ref, int* total){
	node *temp = *head_ref;
	*head_ref = (*head_ref)->next;
	event job = temp->job;
	free(temp);
	(*total)--;
	return job;
}

