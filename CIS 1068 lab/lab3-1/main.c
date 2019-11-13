//Name: Tuan Nguyen
//lab3
#include <netinet/in.h>
#include <sys/socket.h> 
#include <stdio.h> 
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

#include "workqueue.h"
#include "logqueue.h"

#define default_port 1111 //port number as default
#define default_dic "dictionary.txt" // dictionary name as default
#define DICT_SIZE 1000000  //size of dictionary to read

int listenfd(int); 
char **readDict(char *input);
char *get_input();
char **get_parse(char *input);
void writeLogFile(char *input);
void *run_thread(void *input);


int main(int argc, char *argv[]){
	
	int port; //port number
	char *dic_name;	//dictionary name
	int new_port; //new port number , if the user doesn't want to connect to default port
	char *new_dic;	//new dictionary name
	char **read;	//word that read in dictionary.txt file 

	char *input;
    char **argument;

    struct sockaddr_in client;	//set up socket descriptor 
	int clientLen;	// length of socket 
	int connectionSocket, new_socketfd; 


    printf("Enter: ");
    input = get_input();
    argument = get_parse(input);
	
	if(argument[0] != NULL && argument[1] == NULL){ // ./server
			port = default_port;	//listen default
			dic_name = default_dic;	//dictionary name default
			connectionSocket = listenfd(port); //open socket to listen
			printf("port number listen default: %d\n",port);
			if((read = readDict(dic_name)) == NULL){ 	//check if dictionary load success
				printf("cannot load");
			}
			else{
				printf("success\n");
			};
			printf("dictionary loaded default: %s\n",dic_name);	
			//printf("here\n");
	}
	else if(argument[1] != NULL && argument[2] == NULL){	//./server dictionary.txt
			port = default_port;	//listen default
			new_dic = argument[1];	//new dictionary name , not default
			connectionSocket = listenfd(port);	//open socket to listen
			printf("port number listen default: %d\n",port);
			if((read = readDict(new_dic)) == NULL){	//check if dictionary load success
				printf("cannot load");
			}
			else{
				printf("success\n");
			};
			printf("dictionary loaded new: %s\n",new_dic);	
			//printf("here1\n");
	}
	else{	//./server dictionary.txt 8888
			new_dic = argument[1];	//new dictionary name
			new_port = atoi(argument[2]);	//new port listen
			connectionSocket = listenfd(new_port);	// open socket to listen
			printf("port number listen new: %d\n",new_port);	
			if((read = readDict(new_dic)) == NULL){	////check if dictionary load success
				printf("cannot load");
			}
			else{
				printf("success\n");
			};
			printf("dictionary loaded new: %s\n",new_dic);
			//printf("here2\n");
	}


	pthread_t pool_thread[4]; // make thread poll contain 4 thread

	struct workqueue* que = createQueue(10); //create workqueue with size of 10 
	struct logqueue* log = createLog(10);	//create logqueue with size of 10 

	int index = 0;
	int buffer[index];
	int head = 0;
	int tail = 0;

	while(1){
		clientLen = sizeof(struct sockaddr_in);
		new_socketfd = accept(connectionSocket, (struct sockaddr*)&client, (socklen_t*)&clientLen);
		puts("message");
		
		pthread_mutex_lock(&que->workQueue); // lock queue
			while(que->length == que->size){ // if queue is full
				pthread_cond_wait(&que->full, &que->workQueue);	//wait
			
			}	
				buffer[index] = new_socketfd; // add new_socket into array
				index++;  // increment index
				que->length++;// increment length 
				pthread_cond_signal(&que->empty);	// singal if queue is empty

				pthread_mutex_unlock(&que->workQueue); // unlock to add new queue
		
	close(new_socketfd);	//close socket listen
	}
	return 0;
}

// read dictionary 
char **readDict(char *input){
   //printf("in here 2\n");

    FILE *infile = infile = fopen(input, "r+"); // open dict to read
    
    int index = 0;
    char ch[100]; // size of character to read
    char **word = malloc(DICT_SIZE * sizeof(char *));

        while(fgets(ch,100,infile)){ // read each character
           word[index] = ch; //put character into word array
           index++; //increment index
        }
        fclose(infile);
        word[index] = NULL;

    return word;
}
//read user command input can store in a buffer
char *get_input(){

	int buffer_size = 1000;  //initiated buffer size to 1000
    int character; //store the character that user input

	int position = 0; //the position of character

	char *input = malloc(sizeof(char)*buffer_size); //allocate the buffer

	while(1){

		character = getchar(); //get character
		if(character == EOF || character == '\n'){  //get the character till end of file or end of line
			input[position] = '\0'; // get input position until end of string
			//putchar(input); // put character to outout
			return input;
		}
		else{
			input[position] = character; //if input position equal character
		}
		position++; //increment position

	}
        free(input); //free input after done using
    return input;
}

//parse user input into args which separated by " " space
char **get_parse(char *input){
    char **split_line;
    char *token;
    int sizeOfLine = 100;
    int position = 0;
    split_line = malloc(sizeof(char *) * sizeOfLine); // get the size of line

    token = strtok(input, " "); // use strtok that take input and delimiter to split string into token

    while(token != NULL){ // token is not end of line
        split_line[position++] = token; //increment the position of line
        token = strtok(NULL, " "); //split line into token use when user type cd > << &

    }

    split_line[position] = NULL; //split line until the end of string
    return split_line;
}
//set up the server to listen
int listenfd(int portNum){

	int listenfd, optval=1;
	struct sockaddr_in serveraddr;
	
	/* Create a socket descriptor */
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	} 
	/* Eliminates "Address already in use" error from bind */ 
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int)) < 0){ 
	     return -1; 
	} 
	//Reset the serveraddr struct, setting all of it's bytes to zero. 
	//bind() is then called, associating the port number with the 
	//socket descriptor. 
	bzero((char *) &serveraddr, sizeof(serveraddr)); 
	serveraddr.sin_family = AF_INET; 
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serveraddr.sin_port = htons((unsigned short)portNum); 

	if (bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
	      return -1; 
	 } 
	 puts("bind done");
	 printf("waiting for connect\n");
	 //Prepare the socket to allow accept() calls. The value 20 is 
	 //the backlog, this is the maximum number of connections that will be placed 
	 //on queue until accept() is called again. 
	 if (listen(listenfd, 20) < 0){      
	 	return -1; 
	 } 
	 return listenfd;
}
//write word to log.txt file 
void writeLogFile(char *input){
	char *word;
	FILE *outfile;
	outfile = fopen("log.txt", "w+");

	while(1){
		fputs(word, outfile);
	}
	fclose(outfile);
	
}
//run thread pool and check for word spell
void *run_thread(void *input){
	int newconnect;
	char word[100];	
	char spell[100];
	int found = 0;
	while(1){
		for(int i = 0 ; i < sizeof(word); i++){ //loop through word to check for spell
			if(found == 1){	//word found in dictionary ,
				printf("correct"); // print word correct spelled
			}
			else {	
				printf("misspelled");	//print word misspelled
			}
			write(newconnect, spell, sizeof(spell)); // write word into log.txt file 
		}
		close(newconnect);
	}
}


	
