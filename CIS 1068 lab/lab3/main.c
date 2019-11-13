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
#include <errno.h>

#include "workqueue.h"
#include "logqueue.h"

#define default_port 1111 //port number as default
#define default_dic "dictionary.txt" // dictionary name as default
#define DICT_SIZE 1000000  //size of dictionary to read

int listenfd(int); 
char **readDict(char *input);
char *get_input();
char **get_parse(char *input);
void *run_thread(void *input);
ssize_t readinput(int fd, void *buffer, size_t n);


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

	struct workqueue* que = createQueue(10); //create workqueue with size of 10 
	struct logqueue* logs = createLog(10);	//create logqueue with size of 10 

	int thread_count = 0; //count of thread

	int i;

	struct thread args;
	pthread_t pool_thread[4]; // make thread poll contain 4 thread

	
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

	//create thread 
	args.work = que;
	args.word = read;

	
	for(i = 0 ; i < 4; i++){
		pthread_create(&pool_thread[i] , NULL , &run_thread , &args); // create main thread 	
		}
		

	while(1){
		clientLen = sizeof(struct sockaddr_in);
		new_socketfd = accept(connectionSocket, (struct sockaddr*)&client, (socklen_t*)&clientLen);
		puts("message");
		
		thread_count++; 
		printf("thread serve %d\n",thread_count);
		insertqueue(que, new_socketfd); // insert queue
		
			if(thread_count > 4){ //serve more than 4 thread
			printf("Wait until 1 thread release\n");
		}
	}
		
		for(int i = 0; i < 4; i++){
			pthread_join(pool_thread[i],NULL); // join thread 
		}

	return 0;
}

// read dictionary 
char **readDict(char *input){
   //printf("in here 2\n");

    FILE *infile = infile = fopen(input, "r+"); // open dict to read
    
    int index = 0;
    char ch[1000000]; // size of character to read
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
//run thread pool and check for word spell
void *run_thread(void *input){
	
	int newconnect;
	char **word;	
	char read[100];
	char found_word[100];
	int found = 0;
	char *inputs;
	size_t bytes;

	struct thread *args = input; 
	struct workqueue *que = args->work;
	word = args->word;

	FILE *outfile;
	outfile = fopen("log.txt", "w+");
	while(1){
		newconnect = removequeue(que); // remove the queue from work  queue
		while((bytes = readinput(newconnect , &read , 100)) > 0){
			memset(found_word , 0 , sizeof(found_word));
			for(int i = 0 ; word[i] != NULL ; i++){
				if(strncmp(word[i] , read , strlen(read)) == 0){
					strncpy(found_word , read , strlen(read));
					found = 1;
					break;
				}
			}
			if(found == 1){
				strcat(found_word , " Correct\n");
			}
			else{
				strncpy(found_word , read , strlen(read));
				strcat(found_word , " Misspelled\n");
			}
			write(newconnect , found_word , sizeof(found_word));
			fputs(found_word,outfile);
			found = 0;
		}	
		close(newconnect);
	}
		
}

//read in user input
ssize_t readinput(int fd, void *buffer, size_t n)
{
    ssize_t numRead;                    
    size_t totRead;                     
    char *buf;
    char ch;

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;                       

    totRead = 0;
    while(1) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            if (errno == EINTR)         
                continue;
            else
                return -1;              

        } else if (numRead == 0) {      
            if (totRead == 0)           
                return 0;
            else                       
                break;

        } else {                        
            if (totRead < n) {      
                totRead++;
                *buf++ = ch;
            }

            if (ch == '\n')
                break;
        }
    }

    *buf = '\0';
    return totRead;
}





	
