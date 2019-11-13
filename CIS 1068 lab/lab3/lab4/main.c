#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <stdint.h>

int get_input(char input);
char *read_input();
char **get_parse(char *input);
int fileCommand(char *input[]);
void allocatespace(int);
int root_dir();
void allocateFat();
int write_file(char *input[]);
int read_file(char *input[]);
int ls(char *input[]);
int open_file(char *input[]);
int make_dir(char *input[]);
int close_file(char *input[]);


struct dirEntry{
	char *parent_name; // 2 byte
	char *filename; // 2 byte 
	char *dir_name; // 2 byte
	unsigned short create_time; // 2 byte for creation time 
	unsigned short create_date; // 2 byte for date time 
	unsigned short startCluster; //2 bype for start in FAT
	unsigned long size; //4 bype for size
};

struct fileEntry{
	char *filename; // 2 byte
	unsigned short create_time; // 2 byte for creation time 
	unsigned short create_date;	// 2 byte for date time 
	unsigned short startCluster; //2 bype for start in FAT
	unsigned long size; //4 bype for size
};
struct dataEntry{
	unsigned short start_data;  // 2 byte for start reading from block 13 is data region
	unsigned short end_data; // 2 byte for end reading 
};

struct fatEntry{
	unsigned short data; // next data block of fat entry 2 bytes
};

char *fat_region; // array of fat region 
char *dir_region; // array of directory region
char *data_region; // array of data region 

int main(){
	

	int fd;
	fd = open("Drive2MB", O_RDWR); //open drive 2mb to read and write

	if(fd == -1){
		printf("Cannot open drive\n");
	}
	else {
		printf("Success open drive\n");
	}

	printf("\nFile system run Disk Drive 2MB:\n");
	allocatespace(fd); // allocate spze for FAT table 
	root_dir(); // create root directory 
	
	while(1){
	printf("\nCreate, Delete, Open, Write, Close, Read, mkdir, ls, exit\n");
	printf("/");
	char *read = read_input();  // input for file function use( read, write, close)
	char **argument = get_parse(read); //parse input 
	fileCommand(argument);  

	}
	return 0;
}

//option of file system to use 
int fileCommand(char *argument[]){

	 if(strncmp(argument[0] , "create", 6) == 0){ // command to create file 
	 	printf("Success create : %s \n", argument[1]);
	 	write_file(argument);
	 }
	 else if(strncmp(argument[0] , "read", 4) == 0){	// command to read file 
	 	printf("Success read: %s\n", argument[1]);
	 	read_file(argument);
	 }
	 else if(strncmp(argument[0] , "open", 4) == 0){ // command to open file 
	 	printf("Success open : %s\n",argument[1]);
	 	open_file(argument);
	 }
	 else if(strncmp(argument[0] , "delete", 6) == 0){ // command to delete file 
	 	printf("Success delete: %s\n", argument[1]);
	 }
	 else if(strncmp(argument[0] , "close", 4) == 0){ // command to close file 
	 	printf("file is close\n");
	 	close_file(argument);
	 }
	 else if(strncmp(argument[0] , "mkdir", 5 ) == 0){// command to make directory
	 	make_dir(argument);	
	 }
	 else if(strncmp(argument[0] , "ls", 2) == 0){ // command to list the content of file 
	 		ls(argument);
	 }
	 else if(strncmp(argument[0] , "exit", 2) == 0){ // command to exit file
	 	printf("file is exit\n");
	 	exit(0);
	 }
	 else {
	 	printf("invalid file command\n"); // check for invalid command 
	 }
	 return 0;
}

//read user command input can store in a buffer
char *read_input(){

	int buffer_size = 100;  //initiated buffer size to 100
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

//set current directory  = 0; closes a file descriptor 
//, so that it no longer refers to any file and may be 
//reused
int close_file(char *input[]){
	int currentDir = 0;
	return currentDir;
}
// write to file
int write_file(char *input[]){
	char word;
	FILE *writefile;
	writefile =fopen(input[1], "w+");
	
	printf("write: ");
	while(word != '\n'){
		word = getchar();
		fputc(word,writefile);
	}
	fclose(writefile);
	return 0;
}

// read to file
int read_file(char *input[]){

	int ch = 100;
	FILE *readfile;

	readfile = fopen(input[1], "r+");
	printf("read: ");

	while((ch = getc(readfile)) != EOF){
		putchar(ch);
	}
	return 0;
}

//open file 
int open_file(char *input[]){

	struct dataEntry data_entry; // a struct for dataEntry 
	int start_data = 0;  
	int end_data = 0;
	int ch = 100;

	int fileindex;
	FILE *openfile;
	openfile = fopen(input[1], "r+");

	data_entry.start_data = 13; // record the open file at location block 13
	data_entry.end_data = start_data + 1; // end of data will be the start of data + 1 block 
	printf("location of file: %d\n",data_entry.start_data); // print the location the file store

	openfile = fopen(input[1], "r+");  // read the file
	printf("content: ");

	while((ch = getc(openfile)) != EOF){	//read till end of file
		putchar(ch); // print content of file into screen 
	}
	return 0;
}

//create new directory with pathname
int make_dir(char *input[]){

	struct dirEntry dir_entry;
	struct fatEntry fat_entry;
	char dir_buffer[100];  //allocate buffer to dir_entry 
	char fat_buffer[100];  //allocate buffer to fat_entry 
	int status = 0; // check for success create directory 
	
	for(int i = 0; input[i] != '\0'; i++){ // loop through input create directory
		if(status == 0){ // success create directroy
			printf("Create a directory : %s", input[1]);
			dir_entry.dir_name = "";
			dir_entry.create_time = time(NULL);
			dir_entry.create_date = time(NULL);
			dir_entry.startCluster = 11; 
			dir_entry.size = 0;	
			break;
		}
		else{
			printf("Cannot create directory");
		}
	}

	memcpy(&dir_buffer[12], &dir_entry, sizeof(struct dirEntry)); // memory copy new directory create to dir_region
	memcpy(&fat_buffer[0], &fat_entry, sizeof(struct fatEntry)); // also, memory copy to fat table , and update
	return 0;
}

// allocate space to map the fat_region, dir_region, and data_region 
void allocatespace(int fd){

	//char file1 = "hello"
	//char file2 = "hi"
	char root[] = "/";
	//map fat region 
	fat_region = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED,fd,0); // map first 10 block (2^10) fat , page size 1024
	if(fat_region == MAP_FAILED){ //check if map fat_region success
		printf("cannot map fat\n");
	}
	else{
		printf("Success map fat\n"); 
	}

	for( int i = 0; i < 1024; i++){ // for loop through fat entry 
		//printf("word: %c , at location %d\n",word[i], i);
		fat_region[i] = 0; // set each fat entry to 0 because it didn't store anything yet 
	}
	//map directory region 
	dir_region = mmap(NULL, 32 , PROT_READ | PROT_WRITE, MAP_SHARED, fd,4096);  //next 2 block (2^12) dir_table
	if(dir_region == MAP_FAILED){ 
		printf("cannot map dir table\n");
	}
	else{
		printf("Success map dir\n");
	}
	for(int i = 0 ; i < 32; i++){
		dir_region[i] = 0; // set each dir entry to 0 because it didn't store anything yet 
	}
	memcpy(&dir_region[0], &root, sizeof(char)); // write root to dir_region

	//map data region
	data_region = mmap(0,2097152, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 8192); // next block (2^13) to 8192 store data
	if(data_region == MAP_FAILED){ 
		printf("cannot map data\n");
	}
	else{
		printf("Success map data\n");
	}
	//strcpy(data_region, "1234");
	//printf("region: %s\n",data_region);
	for(int i  = 0; i < 20971; i++){ 
		data_region[i] = 0; // set each data entry to 0 because it didn't store anything yet 
	}
	close(fd);
}

//print root directory
int root_dir(){
	struct dirEntry root_entry;
	struct fatEntry fat_entry;
	char fat_buffer[100];
	char dir_buffer[100];
	char current;

	current = '.'; //current working directory

	//information of root directory create 
	root_entry.parent_name = NULL;
	root_entry.filename = "/";
	root_entry.create_time = time(0);
	root_entry.create_date = time(0);
	root_entry.startCluster = 11; //start dir entry at block 11
	root_entry.size = 0;
	
	memcpy(&fat_buffer[11], &fat_entry, sizeof(struct fatEntry)); // memory copy fat region
	memcpy(&dir_buffer[0],&root_entry, sizeof(struct dirEntry)); // memory copy dir region 
	return 0;
}
// list the content of dir
int ls(char *input[]){
	DIR *dir;
    struct dirent *s;
    char *dirname;
    	
    	if(input[1] == NULL){ // specify a file 
    		dirname = ".";
    	 	printf("Success list content of current directory\n");  //command: dir 

                dir = opendir(dirname);
                while((s = readdir(dir)) != NULL){

                printf("%s\n",s ->d_name);

                }
                closedir(dir);
    	}
    	else {
    		dirname = input[1];
    		printf("Success list content of directory: %s\n",input[1]); //command: dir foldername
                    dir = opendir(dirname);

                while((s = readdir(dir)) != NULL){
                     printf("%s\n",s ->d_name);
                    }
                     closedir(dir);
    		}
    	return 0;
}








