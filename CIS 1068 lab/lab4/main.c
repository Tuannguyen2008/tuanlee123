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

#define BLOCK_SIZE 512
#define FILE_MAX 64
#define START_OF_FAT 0
#define START_OF_ROOT 10
#define START_OF_DATA 13
#define FAT_SIZE 2048
#define DIR_TABLE_SIZE 32
#define DATA_SIZE 1048576

int fileCommand(char str[]);
void allocatespace(int);
void root_dir();
void allocateFat();
void read_file(char *filename);
void ls();
void initEntry(int index);
void create_file(char *filename, int flag);
int findfreeFAT();
int findFreeEntry();
int findFreeBlock();
int searchFile(char *filename);
void delete_file(char *filename);
void write_file(char *filename , char *writeData);
int open_file(char *filename);
void close_file();

struct FAT{
	uint16_t next; //2 bytes
};

struct Entry{
	char *name; //11 bytes
	uint16_t create_time; //2 bytes
	uint16_t last_access; //2 bytes
	uint16_t modified_time; //2 bytes
	uint16_t start_blocks; //2 bytes
	uint32_t size; //4 bytes
};

struct FAT *fatMap;
struct Entry *dirTable;
char *dataMap;
int currentDIR; 

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
	char argument[100];
	
	while(1){
	printf("\nCreate, Delete, Open, Write, Close, Read, mkdir, ls, exit\n");
	printf("/");
	scanf("%s",argument);
	fileCommand(argument);  

	}
	return 0;
}


//option of file system to use 
int fileCommand(char str[]){

	char fileName[11];
	int firstBlock;

	time_t current;
	time(&current);


	if (strcmp(str, "create") == 0){ 
		printf("Enter the name of file to create: ");
		scanf("%s", fileName);
		create_file(fileName, 0);		
		printf("Successfully create: %s\n",fileName);

		char *dataBlock = calloc(10000, sizeof(char));	// create an empty larger array to write
		printf("current time : %s", ctime(&current));

		printf("--- Write --- \n");
		
		if(read(STDIN_FILENO, dataBlock, 10000) < 0){ // read standard input
		}
		write_file(fileName, dataBlock); // write to file 
		free(dataBlock);
	}
	else if (strcmp(str, "delete") == 0){				// delete files
		printf("Enter the name of file to delete: ");
		scanf("%12s", fileName);
		delete_file(fileName); 
		
	}
	else if (strcmp(str, "read") == 0){					// read data blocks of a file
		printf("Enter the name of file to read: ");
		scanf("%12s", fileName);
		read_file(fileName);
		printf("current time : %s", ctime(&current));
	}
	
	else if (strcmp(str, "mkdir") == 0){  				// create a directory
		printf("Enter the name of directory to make: ");
		scanf("%s", fileName);
		create_file(fileName, 0); 						
		printf("Successfully create directory: %s\n",fileName);
		printf("current time : %s", ctime(&current));
	}

	else if (strcmp(str, "ls") == 0){					 // list content of  current dir
			printf("Display content of current directory");
			printf("\n");
			ls();	
	}
	else if(strcmp(str , "open") == 0){			//open to read
		printf("Enter the file name to open: ");
		scanf("%12s", fileName);
		firstBlock = open_file(fileName);
		printf("First block of the file is: \n"); // location of file 
		printf("%d\n" , firstBlock);
		read_file(fileName);
		printf("current time : %s", ctime(&current));
	}
	else if(strcmp(str , "exit") == 0){	// exit program
		printf("file is exit\n");
		printf("current time : %s", ctime(&current));
		exit(0);
	}
	else if (strcmp(str, "close") == 0){	// close file		
		close_file();
		printf("Successfully close the file \n");
	}	
	else {
		printf("Invalid Command\n");
	}
	 
	 return 0;
}
// allocate space to map the fat_region, dir_region, and data_region 
void allocatespace(int fd){

	char root[] = "/";
	//map fat region 
	fatMap = mmap(NULL, FAT_SIZE *sizeof(struct FAT), PROT_READ | PROT_WRITE, MAP_SHARED,fd,0); // map first 10 block (2^10) fat , page size 1024
	if(fatMap == MAP_FAILED){ //check if map fat_region success
		printf("cannot map fat\n");
	}
	else{
		printf("Success map fat\n"); 
	}
	for(int i = 0 ; i < FAT_SIZE ; i++){
		fatMap[i].next = 0x0000;
	}

	//map directory region 
	dirTable= mmap(NULL, DIR_TABLE_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED, fd,4096);  //next 2 block (2^12) dir_table
	if(dirTable == MAP_FAILED){ 
		printf("cannot map dir table\n");
	}
	else{
		printf("Success map dir\n");
	}
	for(int i = 0 ; i < DIR_TABLE_SIZE  ; i ++){
		initEntry(i);
	}

	memcpy(&dirTable[0], &root, sizeof(char)); // write root to dir_region
	//map data region
	dataMap = mmap(0,DATA_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 8192); // next block (2^13) to 8192 store data
	if(dataMap == MAP_FAILED){ 
		printf("cannot map data\n");
	}
	else{
		printf("Success map data\n");
	}
	for(int i = 0 ; i < DATA_SIZE ; i++){
		dataMap[i] = 0;
	}

	close(fd);
}
//create root;
void root_dir(){

	struct FAT fatRoot;
	struct Entry rootEntry;
	char name;
	currentDIR = 0;

	fatRoot.next = 0xFFFF;
	name = '.';

	rootEntry.name = "/";
	rootEntry.create_time = time(0);
	rootEntry.last_access = time(0);
	rootEntry.modified_time = time(0);
	rootEntry.start_blocks = START_OF_DATA; // block 10
	rootEntry.size = 0;

	memcpy(&fatMap[START_OF_DATA] , &fatRoot , sizeof(struct FAT));
	memcpy(&dirTable[0] , &rootEntry , sizeof(struct Entry));
	memcpy(&dataMap[0] , &name , sizeof(char));
}

//init an entry with dir table index
void initEntry(int index){

	struct Entry dirEntry;
	
	dirEntry.name = "";
	dirEntry.create_time = 0;
	dirEntry.last_access = 0;
	dirEntry.modified_time = 0;
	dirEntry.start_blocks = 0xFFFF;
	dirEntry.size = 0;

	memcpy(&dirTable[index] , &dirEntry , sizeof(struct Entry));
}

//create a file or a directory
void create_file(char *filename, int flag){
	struct Entry dirEntry;
	struct FAT fat;
	int entryIndex, dirFree, blockFree;
	char ch;
	ch = '.';

	if((entryIndex = searchFile(filename)) != -2){
		printf("file is already exist\n");
		return;
	}
	if((dirFree = findFreeEntry()) == -2){ // find free block in directory table 
		printf("Directory table is out of space\n");
	}
	if((blockFree = findFreeBlock()) == -2){ // find free block in data table
		printf("cannot find a block that is not full\n");
	}
	//create a new dir entry
	dirEntry.name = filename;
	dirEntry.create_time = time(0);
	dirEntry.last_access = time(0);
	dirEntry.modified_time = time(0);
	dirEntry.start_blocks = blockFree;
	dirEntry.size = 0;

	fat.next = 0xFFFF;
	//update dirtable, FAT, and data
	memcpy(&dirTable[dirFree] , &dirEntry , sizeof(struct Entry));
	memcpy(&fatMap[blockFree] , &fat , sizeof(struct FAT));
	memcpy(&dataMap[(blockFree - START_OF_DATA) * 512] , &ch , sizeof(char));
	//add in currentDir
	for(int i = currentDIR ; i < currentDIR + 512 ; i += 32){
		if(dataMap[i] == 0){
			memcpy(&dataMap[i] , &filename[0] , sizeof(char));
			memcpy(&dataMap[i + 1] , &filename[1] , sizeof(char));
			memcpy(&dataMap[i + 2] , &filename[2] , sizeof(char));
			memcpy(&dataMap[i + 3] , &filename[3] , sizeof(char));
			memcpy(&dataMap[i + 4] , &filename[4] , sizeof(char));
			memcpy(&dataMap[i + 5] , &filename[5] , sizeof(char));
			memcpy(&dataMap[i + 6] , &filename[6] , sizeof(char));
			memcpy(&dataMap[i + 7] , &filename[7] , sizeof(char));
			memcpy(&dataMap[i + 8] , &filename[8] , sizeof(char));
			memcpy(&dataMap[i + 9] , &filename[9] , sizeof(char));
			memcpy(&dataMap[i + 10] , &filename[10] , sizeof(char));
			break;
		}                      
	}
	//if file is directory, go to that directory
	if(flag == 1){
		currentDIR = (blockFree - START_OF_DATA) * 512;
	}
}

void read_file(char *filename){
	int dirIndex, fatIndex, index, blockStart , blockEnd;
	char buffer[512];
	int j = 0;
	if((dirIndex = searchFile(filename)) == -2){
		printf("File cannot be found\n");
		return;
	}
	//index in FAT of the file
	fatIndex = dirTable[dirIndex].start_blocks;
	blockStart = (fatIndex - START_OF_DATA) * 512;
	blockEnd = blockStart + 512;
	for(int i = blockStart ; i < blockEnd ; i++ , j++){ //go through each blocks one by one, 
		memcpy(&buffer[j] , &dataMap[i] , sizeof(char));//read bytes to the buffer of size 512
		printf("%c" , buffer[j]);
	}
	index = fatMap[fatIndex].next;

	while(index != 0xFFFF){
		j = 0;
		blockStart = (index - START_OF_DATA) * 512;
		blockEnd = blockStart + 512;
		for(int i = blockStart ; i < blockEnd ; i++ , j++){
			memcpy(&buffer[j] , &dataMap[i] , sizeof(char));
			printf("%c" , buffer[j]);
		}
		index = fatMap[index].next;
	}
	printf("\n");
}
void ls(){
	char filename[11];
	for(int i = currentDIR ; i < currentDIR + 512 ; i += 32){
		memcpy(&filename[0] , &dataMap[i] , sizeof(char));
		memcpy(&filename[1] , &dataMap[i + 1] , sizeof(char));
		memcpy(&filename[2] , &dataMap[i + 2] , sizeof(char));
		memcpy(&filename[3] , &dataMap[i + 3] , sizeof(char));
		memcpy(&filename[4] , &dataMap[i + 4] , sizeof(char));
		memcpy(&filename[5] , &dataMap[i + 5] , sizeof(char));
		memcpy(&filename[6] , &dataMap[i + 6] , sizeof(char));
		memcpy(&filename[7] , &dataMap[i + 7] , sizeof(char));
		memcpy(&filename[8] , &dataMap[i + 8] , sizeof(char));
		memcpy(&filename[9] , &dataMap[i + 9] , sizeof(char));
		memcpy(&filename[10] , &dataMap[i + 10] , sizeof(char));

		printf("%s ", filename);
	}
		printf("\n");
}
void delete_file(char *filename){
	int temp, dirIndex,fatIndex, index, blockStart , blockEnd;
	struct Entry emptyEntry;
	char entryName[11] , ch;
	ch = 0;
	if((dirIndex = searchFile(filename)) == -2){
		printf("File cannot be found\n");
		return;
	}
	fatIndex = dirTable[dirIndex].start_blocks; //fat index of first block

	emptyEntry.name = "";
	emptyEntry.create_time = 0;
	emptyEntry.modified_time = 0;
	emptyEntry.last_access = 0;
	emptyEntry.start_blocks = 0xFFFF;
	emptyEntry.size = 0;
	//replace entry in dir table with empty entry
	memcpy(&dirTable[dirIndex] , &emptyEntry , sizeof(struct Entry));

	index = fatMap[fatIndex].next;
	fatMap[fatIndex].next = 0x0000;
	//delete data in the FAT & in dataMap
	blockStart = (fatIndex - START_OF_DATA) * 512;
	blockEnd = blockStart + 512;
	for(int i = blockStart ; i < blockEnd ; i++){
			dataMap[i] = 0;
		}

	while(index != 0xFFFF){
		blockStart = (index - START_OF_DATA) * 512;
		blockEnd = blockStart + 512;
		for(int i = blockStart ; i < blockEnd ; i++){
			dataMap[i] = 0;
		}
		temp = fatMap[index].next;
		fatMap[index].next = 0x0000;
		index = temp;
	}
	//delete in the current directory
	for(int i = currentDIR ; i < currentDIR + 512 ; i += 32){
		memcpy(&entryName[0] , &dataMap[i] , sizeof(char));
		memcpy(&entryName[1] , &dataMap[i + 1] , sizeof(char));
		memcpy(&entryName[2] , &dataMap[i + 2] , sizeof(char));
		memcpy(&entryName[3] , &dataMap[i + 3] , sizeof(char));
		memcpy(&entryName[4] , &dataMap[i + 4] , sizeof(char));
		memcpy(&entryName[5] , &dataMap[i + 5] , sizeof(char));
		memcpy(&entryName[6] , &dataMap[i + 6] , sizeof(char));
		memcpy(&entryName[7] , &dataMap[i + 7] , sizeof(char));
		memcpy(&entryName[8] , &dataMap[i + 8] , sizeof(char));
		memcpy(&entryName[9] , &dataMap[i + 9] , sizeof(char));
		memcpy(&entryName[10] , &dataMap[i + 10] , sizeof(char));	

		if(strcmp(entryName , filename) == 0){
			memcpy(&dataMap[i] , &ch , sizeof(char));
			memcpy(&dataMap[i + 1] , &ch , sizeof(char));
			memcpy(&dataMap[i + 2] , &ch , sizeof(char));
			memcpy(&dataMap[i + 3] , &ch , sizeof(char));
			memcpy(&dataMap[i + 4] , &ch , sizeof(char));
			memcpy(&dataMap[i + 5] , &ch , sizeof(char));
			memcpy(&dataMap[i + 6] , &ch , sizeof(char));
			memcpy(&dataMap[i + 7] , &ch , sizeof(char));
			memcpy(&dataMap[i + 8] , &ch , sizeof(char));
			memcpy(&dataMap[i + 9] , &ch , sizeof(char));
			memcpy(&dataMap[i + 10] , &ch , sizeof(char));		
		}
	}
	printf("Successfully delete: %s\n",filename);
}

void write_file(char *filename , char *writeData){
	int dirIndex, fatIndex, dataSize, blockStart , blockEnd, index, freeBlock, temp;
	int size = 0;
	struct Entry dirEntry;
	if((dirIndex = searchFile(filename)) == -2){
		printf("File cannot be found\n");
		return;
	}
	dataSize = strlen(writeData);
	//find the FAT index of the file
	fatIndex = dirTable[dirIndex].start_blocks;
	temp = fatIndex;
	index = fatMap[fatIndex].next;
	//go to the end block
	while(index != 0xFFFF){
		temp = index;
		index = fatMap[index].next;
	}
	//find a new free block and update FAT
	if((freeBlock = findFreeBlock()) == -2){
		printf("cannot find block that is not full\n");
	}
	//freeBlock = freeBlock - START_OF_DATA;
	fatMap[temp].next = freeBlock;
	temp = freeBlock;
	fatMap[freeBlock].next = 0xFFFF;
	//continue to write until the end of the string
	blockStart = (freeBlock - START_OF_DATA) * 512;
	blockEnd = blockStart + 512;
	for(int i = blockStart ; i < blockEnd ; i++ , size++){
		memcpy(&dataMap[i] , &writeData[size] , sizeof(char));
	}
	dataSize -= size;

	while(dataSize > 0){
		if((freeBlock = findFreeBlock()) == -2){
			printf("cannot find a block that is not full\n");
		}
		//freeBlock = freeBlock - START_OF_DATA;
		fatMap[temp].next = freeBlock;
		fatMap[freeBlock].next = 0xFFFF;
		temp = fatMap[temp].next;

		dataSize += size;
		blockStart = (freeBlock - START_OF_DATA) * 512;
		blockEnd = blockStart + 512;
		for(int i = blockStart ; i < blockEnd ; i++ , size++){
			memcpy(&dataMap[i] , &writeData[size] , sizeof(char));
		}
		dataSize -= size;		
	}	
		//update meta data
		dirEntry.name = filename;
		dirEntry.create_time = time(0);
		dirEntry.last_access = time(0);
		dirEntry.modified_time = time(0);
		dirEntry.start_blocks = fatIndex;
		dirEntry.size = strlen(writeData);

		memcpy(&dirTable[dirIndex] , &dirEntry , sizeof(struct Entry));
}
int open_file(char *filename){
	int dirIndex, fatIndex;
	if((dirIndex = searchFile(filename)) == -2){
		printf("File cannot be found\n");
	}

	fatIndex = dirTable[dirIndex].start_blocks;
	return fatIndex;
}

//find free entry in FAT
int findfreeFAT(){
	for(int i = 0 ; i < FAT_SIZE ; i ++){
		if(fatMap[i].next == 0x0000){
			return i;
		}
	}
	return -2;
}

//find free entry in Directory Table
int findFreeEntry(){
	for(int i = 0 ; i < DIR_TABLE_SIZE ; i ++){
		if(strcmp(dirTable[i].name , "") == 0){
			return i;
		}
	}
	return -2;
}

//find free data Block, return the block number
int findFreeBlock(){
	for(int i = 0 ; i < DATA_SIZE ; i += 512){
		if(dataMap[i] == 0){
			return START_OF_DATA + i / 512;
		}
	}
	return -2;
}

//search for the filename in the root directory
int searchFile(char *filename){
	char cmpName[11];
	for(int i = 0 ; i < 512 ; i += 32){
		if((dataMap[i] != 0) && (dataMap[i] != '.')){

			memcpy(&cmpName[0] , &dataMap[i] , sizeof(char));
			memcpy(&cmpName[1] , &dataMap[i + 1] , sizeof(char));
			memcpy(&cmpName[2] , &dataMap[i + 2] , sizeof(char));
			memcpy(&cmpName[3] , &dataMap[i + 3] , sizeof(char));
			memcpy(&cmpName[4] , &dataMap[i + 4] , sizeof(char));
			memcpy(&cmpName[5] , &dataMap[i + 5] , sizeof(char));
			memcpy(&cmpName[6] , &dataMap[i + 6] , sizeof(char));
			memcpy(&cmpName[7] , &dataMap[i + 7] , sizeof(char));
			memcpy(&cmpName[8] , &dataMap[i + 8] , sizeof(char));
			memcpy(&cmpName[9] , &dataMap[i + 9] , sizeof(char));
			memcpy(&cmpName[10] , &dataMap[i + 10] , sizeof(char));
			if(strcmp(cmpName , filename) == 0){
				return i / 32;
			}
		}
	}
	return -2;
}
void close_file(){
	currentDIR = 0;
}





