#include<stdio.h>
#include<stdlib.h>
#include"fifoqueue.h"
#include"priorityqueue.h"
#include"otherFunction.h"
#include<time.h>


int main(int argc, char **argv){

fclose(fopen("out.txt", "w"));
//Event Lists
const int ARRIVE = 0;
const int CPU_FINISHED = 1;
const int DISK_ARRIVED = 2;
const int DISK_1_FINISHED = 3;
const int DISK_2_FINISHED = 4;
const int EXITS = 5;
const int SIM_END = 6;

//Hyperparameters
int SEED = searchValue("file.txt" , "SEED");
int FIN_TIME = searchValue("file.txt", "FIN_TIME");
int INIT_TIME = searchValue("file.txt", "INIT_TIME");
int QUIT_PROB = searchValue("file.txt", "QUIT_PROB");
int ARRIVE_MIN = searchValue("file.txt", "ARRIVE_MIN");
int ARRIVE_MAX = searchValue("file.txt", "ARRIVE_MAX");
int CPU_MIN = searchValue("file.txt" , "CPU_MIN");
int CPU_MAX = searchValue("file.txt" , "CPU_MAX");
int DISK1_MIN = searchValue("file.txt" , "DISK1_MIN");
int DISK1_MAX = searchValue("file.txt" , "DISK1_MAX");
int DISK2_MIN = searchValue("file.txt" , "DISK2_MIN");
int DISK2_MAX = searchValue("file.txt" , "DISK2_MAX");
srand(SEED);

//Initialize priority queue
Node* JobHandler = newNode(INIT_TIME, ARRIVE,0);
push(&JobHandler, FIN_TIME,SIM_END,10000);

//Create 3 fifo queues
Queue* CPUqueue = initializeJobQueue();
Queue* Disk1 = initializeJobQueue();
Queue* Disk2 = initializeJobQueue();

int currentJobTime = 0;

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



//processing simulation
while((isEmpty(JobHandler) == 0) &&  currentJobTime <= FIN_TIME){
    //Pop job from priority queue
    Node* currentJob = pop(&JobHandler);

    //event is job arrive at a CPU
    if(currentJob->eventID == ARRIVE){

      int arrived_log_in=currentJobTime;
      int ARRIVE_TIME = randomInt(ARRIVE_MIN, ARRIVE_MAX);
      Enqueue(CPUqueue , currentJobTime, currentJob->eventID , currentJob->jobID);
      currentJobTime += ARRIVE_TIME;
      push(&JobHandler, currentJobTime , ARRIVE, create_job());

      if((CPUqueue->state == 0) && (CPUqueue->size > 0)){

        ffNode* CPUNode = Deque(CPUqueue);
        int FINISH = randomInt(CPU_MIN , CPU_MAX);

        currentJobTime += FINISH;
        CPUrunningTime += FINISH;

        push(&JobHandler , currentJobTime, CPU_FINISHED , CPUNode->jobID);
        CPUqueue->state = 1;

        //Find the time job enter the queue to the time job leave the CPU
        CPUtotalResponding+=(currentJobTime-arrived_log_in);
        CPUcount++;
        }

        //Compare CPU max with current CPU size
        if(CPUmax<CPUqueue->size)
          CPUmax= CPUqueue->size;
        //Find average size
        CPUtotalsize +=CPUqueue->size;


    //Event is job finish at CPU
}

    else if(currentJob->eventID==CPU_FINISHED){

        CPUqueue->state = 0;
        int QUIT = quit(QUIT_PROB);
        if(QUIT == 1){
          currentJob->eventID = EXITS;
          appendToFile(currentJob->time , currentJob->eventID , currentJob->jobID , "out.txt");
          jobFinished ++;
        }
        else{
          push(&JobHandler , currentJob->time , DISK_ARRIVED , currentJob->jobID);
        }
}   //Event is job arrive at Disk
    else if(currentJob->eventID == DISK_ARRIVED){

        int arrived_Disk_log_in=currentJobTime;

        if(Disk1->size <= Disk2->size){
          Enqueue(Disk1 , currentJob->time ,currentJob->eventID, currentJob->jobID);

          if(Disk1->size > 0){

            ffNode* FINISH_IO_1 = top(Disk1);
            int FIN_1 = randomInt(DISK1_MIN , DISK1_MAX);
            currentJobTime += FIN_1;
            Disk1runningTime += FIN_1;

            push(&JobHandler , currentJobTime , DISK_1_FINISHED ,FINISH_IO_1->jobID);

            Disk1totalsize +=Disk1->size;
            Disk1count++;

            Disk1totalResponding+=(currentJobTime-arrived_Disk_log_in);
            if(Disk1max<Disk1->size){
                Disk1max=Disk1->size;
             }

          }

        }

        else if(Disk1->size > Disk2->size){

          Enqueue(Disk2 , currentJob->time , currentJob->eventID, currentJob->jobID);

          if(Disk2->size > 0){

            ffNode* FINISH_IO_2 = top(Disk2);
            int FIN_2 = randomInt(DISK2_MIN , DISK2_MAX);
            currentJobTime += FIN_2;
            push(&JobHandler , currentJobTime , DISK_2_FINISHED , FINISH_IO_2->jobID);

            Disk2totalsize +=Disk2->size;
            Disk2count++;

            Disk2totalResponding+=(currentJobTime-arrived_Disk_log_in);
            if(Disk2max<Disk2->size){
                Disk2max=Disk2->size;
             }
           }

        }
      }
    //Event is job finish at disk
    else if(currentJob->eventID == DISK_1_FINISHED || currentJob->eventID == DISK_2_FINISHED){
        if(currentJob->eventID == DISK_1_FINISHED){

          push(&JobHandler , currentJob->time , ARRIVE , currentJob->jobID);
        }
        else{

          push(&JobHandler , currentJob->time , ARRIVE , currentJob->jobID);
        }
      }
    //event is simulation end
    else if(currentJob->eventID == SIM_END) currentJobTime += FIN_TIME;

  }

  printf("\n\n--Statistic-- \n");
  printf("\n\nMaximum size:\n");
  printf("CPU: %d\n", CPUmax);//print CPU max
  printf("Disk1: %d\n", Disk1max);
  printf("Disk2 max: %d\n", Disk2max);

  printf("\n\nAverage size:\n");
  float CPUave = CPUtotalsize/(CPUcount-1);//CPU ave size
  printf("CPU: %0.1f\n", CPUave);
  float Disk1ave = Disk1totalsize/Disk1count;
  printf("Disk1: %0.1f\n", Disk1ave);
  float Disk2ave = Disk2totalsize/Disk2count;
  printf("Disk2: %0.1f\n", Disk2ave);

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


