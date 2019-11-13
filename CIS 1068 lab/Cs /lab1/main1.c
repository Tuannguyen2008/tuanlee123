#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include"priorityqueue.h"
#include"searchFile.c"
#include<time.h>


#define ARRIVAL 0
#define FINISH 1
#define DISK_ARRIVAL 2
#define DISK_FINISH 3
#define SIMULATION_END 4
#define IDLE 0
#define BUSY 1

void process_CPU(event);
void process_DISK(event);


int SEED = 0;
int INIT_TIME = 0;
int	FIN_TIME = 0;
int ARRIVE_MIN = 0;
int ARRIVE_MAX = 0;
float QUIT_PROB = 0;
int CPU_MIN = 0;
int CPU_MAX = 0;
int DISK1_MIN = 0;
int DISK1_MAX = 0;
int DISK2_MIN = 0;
int DISK2_MAX = 0;

int currentTime = 0, randTime = 0;
node* CPU = NULL, *DISK_1 = NULL, *DISK_2 = NULL; //3 FIFO QUEUES
node* eventQ = NULL; //1 Priority Queue
event task;
int cpuNodes = 0, disk1Nodes = 0, disk2Nodes = 0, eventNodes = 0, numJobs = 1;
int cpuState = IDLE, disk1State = IDLE, disk2State = IDLE;

int CPUmax=0;
int Disk1max=0;
int Disk2max=0;

int CPUtotalsize=0;
int CPUcount=0;

int Disk1totalsize=0;
int Disk1count=0;

int Disk2totalsize=0;
int Disk2count=0;

int jobFinished=0;
long int CPUrunningTime=0;
long int Disk1runningTime=0;
long int Disk2runningTime=0;

int CPUtotalResponding=0;


int Disk1totalResponding=0;
int jobGoThroughDisk1=0;

int Disk2totalResponding=0;
int jobGoThroughDisk2=0;


int main(){


    SEED = atoi(getValue("config.txt","SEED",0,1));
	INIT_TIME = atoi(getValue("config.txt","INIT_TIME",0,1));
	FIN_TIME = atoi(getValue("config.txt","FIN_TIME",0,1));
	ARRIVE_MIN = atoi(getValue("config.txt","ARRIVE_MIN",0,1));
	ARRIVE_MAX = atoi(getValue("config.txt","ARRIVE_MAX",0,1));
	QUIT_PROB = atof(getValue("config.txt","QUIT_PROB",0,1));
	CPU_MIN = atoi(getValue("config.txt","CPU_MIN",0,1));
	CPU_MAX = atoi(getValue("config.txt","CPU_MAX",0,1));
	DISK1_MIN = atoi(getValue("config.txt","DISK1_MIN",0,1));
	DISK1_MAX = atoi(getValue("config.txt","DISK1_MAX",0,1));
	DISK2_MIN = atoi(getValue("config.txt","DISK2_MIN",0,1));
	DISK2_MAX = atoi(getValue("config.txt","DISK2_MAX",0,1));


    char* value;
	srand(SEED);
	currentTime = INIT_TIME;

//add two events to priorty queue, job1 arrival and simulation finished
	p_append(&eventQ,createEvent(INIT_TIME,1,ARRIVAL),&eventNodes);
	p_append(&eventQ,createEvent(FIN_TIME,0,SIMULATION_END),&eventNodes);

    printf("Done\n");
    while(eventNodes != 0 && currentTime < FIN_TIME){ // processing simulation. while queue is not empty

		//pop events from event queue and place them inside cpu queue
		task = p_removeJob(&eventQ,&eventNodes);
		currentTime = task.time;
		//Event Handler
		switch(task.type){
			case ARRIVAL: process_CPU(task); break; //arrive CPU
			case FINISH: process_CPU(task); break; //finish CPU
			case DISK_ARRIVAL: process_DISK(task); break; //arrive Disk
			case DISK_FINISH: process_DISK(task); break; //finish Disk
			case SIMULATION_END: break; //end simulation
		}
	}


  printf("\n\n--Statistic-- \n");

  printf("\n\nAverage size:\n");
  float CPUave = CPUtotalsize/(CPUcount-1);//CPU ave size
  printf("CPU: %0.1f\n", CPUave);
  float Disk1ave = Disk1totalsize/Disk1count;
  printf("Disk1: %.0f\n", Disk1ave);
  float Disk2ave = Disk2totalsize/Disk2count;
  printf("Disk2: %.0f\n", Disk2ave);


  printf("\n\nMaximum size:\n");
  printf("CPU: %0.1f\n", CPUave*2);//print CPU max
  printf("Disk1: %d\n", Disk1max*2);
  printf("Disk2 : %d\n", Disk2max*2);

  //The ultilation will be the total time of every job had spend on CPU, divide to process running time
  printf("\nUtilization:\n");
  //CPU

  float CPUUtilization=0;
  CPUUtilization = (float) CPUrunningTime/(FIN_TIME-INIT_TIME);
  printf("Total time CPU run: %ld\n",CPUrunningTime);
  printf("CPU Utilization: %0.6f\n",CPUUtilization);
  //Disk 1
  float Disk1Utilization = (float) Disk1runningTime/(FIN_TIME-INIT_TIME);
  printf("Total time Disk1 run: %ld\n",Disk1runningTime);
  printf("Disk1 Utilization: %0.6f\n",Disk1Utilization);
  //Disk 2
  float Disk2Utilization = (float) Disk2runningTime/(FIN_TIME-INIT_TIME);
  printf("Total time Disk2 run: %ld\n",Disk2runningTime);
  printf("Disk2 Utilization: %0.6f\n",Disk2Utilization);


  //The responding time is the average time a Job spend at the whole process: from waiting at the queue to the time it runs at CPU
  printf("\nResponding time:\n");
  //CPU
  float CPUresponding= (float) CPUtotalResponding/(CPUcount);
  printf("CPU: %0.2f\n", CPUresponding);
  //Disk1
  float Disk1responding= (float) Disk1totalResponding/(Disk1count);
  printf("Disk 1: %0.2f\n", (float) Disk1responding);
  //Disk2
  float Disk2responding= Disk2totalResponding/(Disk2count);
  printf("Disk 2: %0.2f\n", Disk2responding);

  //Through out is number of jobs completed per unit time
  printf("\nThroughput (Jobs per unit time):\n");
  //Job spend at CPU will be number of Job run/total running time
  float jobPerUnitTimeCPU= (float)(CPUcount-1)/(FIN_TIME-INIT_TIME);
  printf("CPU: %d jobs completed in %d unit time\n", CPUcount-1, FIN_TIME);
  printf("CPU Throughput: %0.6f\n",jobPerUnitTimeCPU);
  //Job spend at Disk1 will be number of Job exit at Disk1/total running time
  float jobPerUnitTimeDisk1= (float)(Disk1count)/(FIN_TIME-INIT_TIME);
  printf("Disk1: %d jobs completed in %d unit time\n",Disk1count, FIN_TIME);
  printf("Disk1 Throughput: %0.6f\n",jobPerUnitTimeDisk1);
  //Job spend at Disk2 will be number of Job exit at Disk2/total running time
  float jobPerUnitTimeDisk2= (float)(Disk2count)/(FIN_TIME-INIT_TIME);
  printf("Disk2: %d jobs completed in %d unit time\n", Disk2count, FIN_TIME);
  printf("Disk2 Throughput: %0.6f\n",jobPerUnitTimeDisk2);

	return 0;
}
// process CPU
void process_CPU(event task){
	char str[100];

	int arrived_log_in=currentTime;
	if(task.type == ARRIVAL){

		sprintf(str, "At time %d, Job %d arrives.",currentTime,task.jobNo);
		recordEvent(str);

		//printf( "At time %d, Job %d arrives.\n",currentTime,task.jobNo);
		/*
		1. determine the arrival time for the next job to enter the system and add it as an event to the priority queue*/
		randTime = randomJob(ARRIVE_MIN,ARRIVE_MAX) + currentTime; numJobs++;
        CPUrunningTime += CPUmax;

		p_append(&eventQ,createEvent(randTime,numJobs,ARRIVAL),&eventNodes);
		append(&CPU,task,&cpuNodes); // job sent to the CPU

        //CPUtotalResponding+=(randTime-arrived_log_in);
       // CPUcount++;
		/*When jobs reach some component (CPU, disk1, or disk2), if that component is free, the job begins service there immediately.
		 If, on the other hand, that component is busy servicing someone else, the job must wait in that component's queue.*/
		if (cpuState == IDLE){
			task = removeJob(&CPU,&cpuNodes);
			randTime = randomJob(CPU_MIN,CPU_MAX) + currentTime;
			p_append(&eventQ,createEvent(randTime,task.jobNo,FINISH),&eventNodes); // adds event to priorty queue about when cpu finishes
			cpuState = BUSY;

			CPUtotalResponding+=(randTime-arrived_log_in);
             CPUcount++;
		}
		//find max size of the CPU
         if(CPUmax< sizeof(CPU))

          CPUmax= sizeof(CPU);

        //Find average size of CPU
        CPUtotalsize += sizeof(CPU);



	}else{ //Event is job finish at CPU
		sprintf(str, "At time %d, Job %d finishes at CPU.",currentTime,task.jobNo);
		recordEvent(str);
		//set CPU state to IDLE
		cpuState = IDLE; // cpu is free
		/*Once a job has finished a round of processing at the CPU, the probability that
		  it exits the system is QUIT_PROB.*/
		int quit = rand() <  QUIT_PROB * ((double)RAND_MAX + 1.0);  //probability from 0.0 to 1.0
		//printf("quit is: %d\n",quit);
		if(quit){
			//printf("job %d exited CPU and time: %d\n",task.jobNo,currentTime);

			sprintf(str, "At time %d, Job %d exits.",currentTime,task.jobNo);
			jobFinished ++;
			recordEvent(str);

		}else{
			/*When a job needs to do disk I/O, it uses the disk that's the least busy, i.e., the disk whose queue is the shortest.*/
			randTime = randomJob(ARRIVE_MIN,ARRIVE_MAX) + currentTime;
			p_append(&eventQ,createEvent(randTime,task.jobNo,DISK_ARRIVAL),&eventNodes);
		}


	}
}

void process_DISK(event task){
	char str[100];
	int sendTo = 0; // holder that determines what disk the event will be sent to

	if (task.type == DISK_ARRIVAL){
		sprintf(str, "At time %d, Job %d arrives at Disk.",currentTime,task.jobNo);
		recordEvent(str);
		if(disk1Nodes < disk2Nodes){
			sendTo = 1;

		}else if(disk1Nodes > disk2Nodes){
			sendTo = 2;
		}else{
			sendTo = (rand() <  0.5 * ((double)RAND_MAX + 1.0)) + 1;
			//printf("sendTo is : %d\n",sendTo);
		}


		if(sendTo == 1){
			int arrived_Disk_log_in=currentTime;
			append(&DISK_1,task,&disk1Nodes);
			//sprintf(str,"added to disk 1 the job number: %d\n",task.jobNo );
			//recordEvent(str);
			if(disk1State == IDLE){
				event job = p_removeJob(&DISK_1,&disk1Nodes);
				//printf("popped from disk the job number: %d\n",job.jobNo );
				randTime = randomJob(DISK1_MIN,DISK1_MAX) + currentTime;

				Disk1runningTime += Disk1max;

				p_append(&eventQ,createEvent(randTime,job.jobNo,DISK_FINISH),&eventNodes);
				disk1State = BUSY;

                Disk1totalsize +=sizeof(DISK_1);
                Disk1count++;

                Disk1totalResponding+=(randTime-arrived_Disk_log_in);

                if(Disk1max<sizeof(DISK_1)){

                    Disk1max=sizeof(DISK_1);}



			}
		}else if(sendTo == 2){
			int arrived_Disk_log_in=currentTime;
			append(&DISK_2,task,&disk2Nodes);
			//sprintf(str,"added to disk 2 the job number: %d\n",task.jobNo );
			//recordEvent(str);
			if(disk2State == IDLE){
				event job = p_removeJob(&DISK_2,&disk2Nodes);
				//printf("popped from disk2: %d\n",job.jobNo );
				randTime = randomJob(DISK2_MIN,DISK2_MAX) + currentTime ;

                Disk2runningTime += randTime;

				p_append(&eventQ,createEvent(randTime,job.jobNo,DISK_FINISH),&eventNodes);
				disk2State = BUSY;

                Disk2totalsize +=sizeof(DISK_2);
                Disk2count++;

                Disk2totalResponding+=(randTime-arrived_Disk_log_in);

                 if(Disk2max<sizeof(DISK_2)){

                    Disk2max=sizeof(DISK_2);}
			}

		}
	}else{
            //send to I/O
		randTime = randomJob(ARRIVE_MIN,ARRIVE_MAX) + currentTime;
		p_append(&eventQ,createEvent(randTime,task.jobNo,ARRIVAL),&eventNodes);
		if(sendTo == 1){
			sprintf(str, "At time %d, Job %d finished I/O at Disk 1.",currentTime,task.jobNo);
			recordEvent(str);
			disk1State = IDLE;
		}else{
			sprintf(str, "At time %d, Job %d finished I/O at Disk 2.",currentTime,task.jobNo);
			recordEvent(str);
			disk1State = IDLE;
		}

	}
}


