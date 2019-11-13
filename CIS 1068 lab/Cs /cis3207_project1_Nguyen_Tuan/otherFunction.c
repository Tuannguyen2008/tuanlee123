#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//read parameter value from log file
int searchValue(char *fname, char *str) {
	FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	char valueStr[512];
        int value =0;
	
	if((fp = fopen(fname, "r")) == NULL) {
		return(-1);
	}
        
        int i =0;
        int j = 0;
	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, str)) != NULL) {
                    
                    for(i =strlen(str); i< strlen(temp)+1; i++){
                        valueStr[j]=temp[i];
                        j++;
                    }
                    value =atoi(valueStr);
                    find_result++;
                    return value;
		}
		line_num++;
	}

	if(find_result == 0) {
		printf("\nSorry, couldn't find a match.\n");
	}
	
	if(fp) {
		fclose(fp);
	}
   	return(value);
}



//generate random number in a range
int randomInt(int a, int b){ 
    int newNum = ((rand()%(b-a+1))+a);
    return newNum;
}

//determine whether job will quit system or arrive a disk 
int quit(int QUIT_PROB){
    int i=rand()%10;
        if(i<=QUIT_PROB){
            return 1;
        }else{
            return 0;
        } 
}
//increment JobID
int nextJobNumber=1;
int create_job(){
    return nextJobNumber++;
}

//write events to log file
void appendToFile(int time, int eventID, int jobID, char *filename){
    FILE *fp;
    fp = fopen(filename, "a");
    if(fp==NULL){
        printf("Error\n");
        exit(1);
    }
    fprintf(fp, "At time %d job %d ", time,jobID);
    if(eventID==0){
        fprintf(fp, "arrives\n");
    }else if(eventID==1){
        fprintf(fp, "finished at CPU\n");
    }else if(eventID == 2){
        fprintf(fp,"arrived at disk\n");
    }else if(eventID == 3){
        fprintf(fp,"finished at disk 1\n");
    }else if(eventID == 4){
        fprintf(fp,"finished at disk 2\n");
    }else if(eventID == 5){
        fprintf(fp,"exits\n");
    }else if(eventID == 6){
        fprintf(fp," simulation end\n");
    }
    fclose(fp);
}