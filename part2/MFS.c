#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>


#define BUFFERSIZE 104
#define MAXFLOW 104

typedef struct _flow
{
    float arrivalTime;
    float transTime;
    int priority;
    int id;
} flow;


flow flowList[MAXFLOW];   // parse input in an array of flow
flow *queueList[MAXFLOW];  // store waiting flows while transmission pipe is occupied.
int queueSize = 0;
int pipeBusy = 0;
int currFlow;

struct timeval startTime, currTime;
float elapsed;

pthread_t thrList[MAXFLOW]; // each thread executes one flow
pthread_mutex_t trans_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t trans_cvar = PTHREAD_COND_INITIALIZER;

time_t start;

void printQueue(){
    int i;
    for (i = 0; i < queueSize; i++){
        printf("%d:%.2f,%.2f,%d\n", queueList[i]->id, queueList[i]->arrivalTime, queueList[i]->transTime, queueList[i]->priority);
    }
}

void popQueue(){
    int i;
    for(i = 0; i < queueSize; i++){
        queueList[i] = queueList[i+1];
    }
    queueSize--;
}

int flowComp(void *v1, void *v2){
    flow *f1 = (flow *)v1;
    flow *f2 = (flow *)v2;
    if (f1->priority > f2->priority){
        return -1;
    }else if (f1->priority < f2->priority){
        return 1;
    }else{
        if (f1->arrivalTime > f2->arrivalTime){
            return -1;
        }else if (f1->arrivalTime < f2->arrivalTime){
            return 1;
        }else{
            if (f1->transTime > f2->transTime){
                return -1;
            }else if (f1->transTime < f2->transTime){
                return 1;
            }else{
                if (f1->id > f2->id){
                    return -1;
                }else if (f1->id < f2->id){
                    return 1;
                }
            }
        }
    }
}

void sortQueue(){
    flow *temp;
    int i, j;
    for (i = 0; i < queueSize; i++){
        for (j = i + 1; j < queueSize; j++){
            if(flowComp(queueList[i], queueList[j]) == -1){
                temp = queueList[i];
                queueList[i] = queueList[j];
                queueList[j] = temp;
            }
        }
    }
}

void requestPipe(flow *item) {
    //lock mutex
    pthread_mutex_lock(&trans_mtx);
    if (pipeBusy == 0 && queueSize == 0) {
        //unlock mutex
        pipeBusy = 1;
        pthread_mutex_unlock(&trans_mtx);
        return;
    }
    //add item in queue, sort the queue according rules
    queueList[queueSize] = item;
    queueSize++;
    sortQueue();


    // key point here..
    // wait till pipe to be available and be at the top of the queue

    while(pipeBusy == 1 || item != queueList[0]){
        printf("Flow %2d waits for the finish of flow %2d. \n", item->id, currFlow);
        pthread_cond_wait(&trans_cvar, &trans_mtx);
    }
    pipeBusy = 1;
    // update queue
    popQueue();
    //unlock mutex
    pthread_mutex_unlock(&trans_mtx);
}

void releasePipe() {
    //set pipe to not busy, broadcast cvar
    pipeBusy = 0;
    if(queueSize > 0)
      currFlow = queueList[0]->id;
    pthread_cond_broadcast(&trans_cvar);
}

//entry point for each thread created
void *thrFunction(void *flowItem) {

    flow *item = (flow *)flowItem;

    // wait for arrival
    usleep(item->arrivalTime * 100000);
    printf("Flow %2d arrives: arrival time (%.2f), transmission time (%.1f), priority (%2d). \n", item->id, item->arrivalTime / 10, item->transTime / 10, item->priority);

    requestPipe(item);
    currFlow = item->id;
    gettimeofday(&currTime, NULL);
    elapsed = (currTime.tv_sec - startTime.tv_sec) * 1000.0;
    elapsed += (currTime.tv_usec - startTime.tv_usec) / 1000.0;
    printf("Flow %2d starts its transmission at time %.2f. \n", item->id, elapsed/1000);

    // sleep for transmission time
    usleep(item->transTime * 100000);

    releasePipe(item);
    gettimeofday(&currTime, NULL);
    elapsed = (currTime.tv_sec - startTime.tv_sec) * 1000.0;
    elapsed += (currTime.tv_usec - startTime.tv_usec) / 1000.0;
    printf("Flow %2d finishes its transmission at time %.2f. \n", item->id, elapsed/1000);
}

int main(int argc, char *argv[]) {
    gettimeofday(&startTime, NULL);
    int i;

    // file handling
    FILE *fp = fopen(argv[1], "r"); // read file
    if (fp == NULL){
      printf("Error opening file");
      exit (EXIT_FAILURE);
    }

    char buf[BUFFERSIZE];
    char * tok;
    int numFlows;

    // read number of flows
    fgets(buf, sizeof(buf), fp);
    numFlows = atoi(buf);

    for(i = 0; i < numFlows; i++) {
        fgets(buf, sizeof(buf), fp); // read line by line
        flow f;
        tok = strtok(buf, ":");
        f.id = atoi(tok);
        tok = strtok(NULL, ",");
        f.arrivalTime = atof(tok);
        tok = strtok(NULL, ",");
        f.transTime = atof(tok);
        tok = strtok(NULL, ",");
        f.priority = atoi(tok);

        flowList[i] = f;
    }

    fclose(fp); // release file descriptor


    for(i = 0; i < numFlows; i++){
        // create a thread for each flow
        if(pthread_create(&thrList[i], NULL, thrFunction, (void *)&flowList[i]) != 0){
            printf("Error creating thread");
            exit(EXIT_FAILURE);
        }
    }
    //wait for all threads to terminate
    for(i = 0; i < numFlows; i++)
        pthread_join(thrList[i], NULL);

    //destroy mutex & condition variable
    pthread_cond_destroy(&trans_cvar);
    pthread_mutex_destroy(&trans_mtx);
    return 0;
}
