//
// Created by ogi on 30.05.2022.
//

/* Initial version of functions
#include "functs.h"

void initializeThread(struct ThreadInfo *threadArray[], int exeNumber, int threadIndex, int stackSize){

    threadArray[threadIndex]->state = 0;
    threadArray[threadIndex]->threadNumber= threadIndex;
    threadArray[threadIndex]->countNumber = 0;
    threadArray[threadIndex]->exeNumber = exeNumber;

    threadArray[threadIndex]->context.uc_link = &threadArray[0]->context;
    threadArray[threadIndex]->context.uc_stack.ss_sp = malloc(stackSize);
    threadArray[threadIndex]->context.uc_stack.ss_size = stackSize;
    makecontext(&threadArray[threadIndex]->context, (void (*)(void))runThread, 1, threadIndex);
}

int createThread(struct ThreadInfo *threadArray[]){
    int availableIndex = 0;
    for(int i=5;i>0; i--){
        if(threadArray[i]->state == 2 || threadArray[i]->state == -1){
            availableIndex = i;
        }
    }
    if(availableIndex != 0){
        printf("A new thread is created\n");
    }
    else{
        printf("A new thread could not be created\n");
        return 0;
    }
    return availableIndex;
}

void runThread(struct ThreadInfo *threadArray[], int threadIndex){
    threadArray[threadIndex]->state = 1;

    threadArray[threadIndex]->countNumber = threadArray[threadIndex]->countNumber +1;
    printf("Running for Thread: %d, execution count: %d \n", threadArray[threadIndex]->threadNumber, threadArray[threadIndex]->countNumber);
    sleep(1);

    if(threadArray[threadIndex]->exeNumber == threadArray[threadIndex]->countNumber) {
        exitThread(*threadArray,threadIndex);
    }

    threadArray[threadIndex]->countNumber = threadArray[threadIndex]->countNumber +1;
    printf("Running for Thread: %d, execution count: %d \n", threadArray[threadIndex]->threadNumber, threadArray[threadIndex]->countNumber);

    threadArray[threadIndex]->state = 0;

    if(threadArray[threadIndex]->exeNumber == threadArray[threadIndex]->countNumber){
        exitThread(*threadArray, threadIndex);
    }
}

void exitThread(struct ThreadInfo *threadArray[], int threadIndex){
    free(threadArray[threadIndex]->context.uc_stack.ss_sp);
    threadArray[threadIndex]->state = -1;
}

bool isAllFinished(struct ThreadInfo *threadArray[]){
    for(int i=1; i<6; i++){
        if(threadArray[i]->state != -1){
            return false;
        }
    }
    return true;
}
 */

/* The last version of main, with randomness and flags bugs
#include "ThreadInfo.h"


//#define finished -1
//#define ready 0
//#define running 1
//#define empty 2
#define stackSize 4096

struct ThreadInfo threadArray[6];
int runningThreadIndex  = 0;

void exitThread(int threadIndex) {
    free(threadArray[threadIndex].context.uc_stack.ss_sp);
    threadArray[threadIndex].state = -1;
}

void runThread(int threadIndex){
    threadArray[threadIndex].state = 1;
    threadArray[threadIndex].countNumber = threadArray[threadIndex].countNumber +1;

    for(int i=0; i<threadIndex - 1; i++){
        printf("\t");
    }
    printf("%d, %d \n", threadArray[threadIndex].countNumber, threadArray[threadIndex].state);
    sleep(1);
    threadArray[threadIndex].countNumber = threadArray[threadIndex].countNumber +1;

    for(int i=0; i<threadIndex - 1; i++){
        printf("\t");
    }
    printf("%d, %d \n", threadArray[threadIndex].countNumber, threadArray[threadIndex].state);
    threadArray[threadIndex].state = 0;

    if(threadArray[threadIndex].countNumber >= threadArray[threadIndex].exeNumber){
        exitThread(threadIndex);
        //swapcontext(&threadArray[threadIndex].context,&threadArray[0].context );
    }
    sleep(1);
}

void initializeThread(int exeNumber, int threadIndex){

threadArray[threadIndex].state = 0;
threadArray[threadIndex].threadNumber= threadIndex;
threadArray[threadIndex].countNumber = 0;
threadArray[threadIndex].exeNumber = exeNumber;

getcontext(&threadArray[threadIndex].context);
threadArray[threadIndex].context.uc_link = &threadArray[0].context;
threadArray[threadIndex].context.uc_stack.ss_sp = malloc(stackSize);
threadArray[threadIndex].context.uc_stack.ss_size = stackSize;
makecontext(&threadArray[threadIndex].context, (void (*)(void))runThread, 1, threadIndex);
}

int createThread(int threadIndex){
    int returnObject = getcontext(&threadArray[threadIndex].context);
    if(returnObject == -1) {
        printf("Could not create thread object");
    }

    int availableIndex = 0;
    for(int i=5;i>0; i--){
        if(threadArray[i].state == 2 || threadArray[i].state == -1){
            availableIndex = i;
        }
    }
    if(availableIndex != 0){
        return availableIndex;
    }
    else{
        printf("A new thread could not be created\n");
        return -1;

}

bool isAllFinished(){
    for(int i=1; i<6; i++){
        if(threadArray[i].state != -1){
            return false;
        }
    }
    return true;
}

void handler() {
    swapcontext(&threadArray[runningThreadIndex].context, &threadArray[0].context);
}
void scheduler_one(int initial){
    //printf("schedulera girdi");
    bool isFound = false;
    int randomNumber;
    while (!isFound) {
        randomNumber = random() % 5 + 1;
        if (threadArray[randomNumber].countNumber < threadArray[randomNumber].exeNumber) {
            break;
        }
    }
    runningThreadIndex = randomNumber;
    alarm(2);
    swapcontext(&threadArray[0].context, &threadArray[randomNumber].context);
}

int main(int argc, char **argv){
    srand(time(NULL));
    for(int i=1; i<6; i++){
        threadArray[i].state = 2;
    }
    int exeNumberArray[6];
    exeNumberArray[0] = 0;
    int total = 0;
    for(int i=1; i<6; i++){
        exeNumberArray[i] = atoi(argv[i]);
        total = total + exeNumberArray[i];
    }
    int threadIndex;
    for(int i=1; i<6; i++){
        createThread(i);
        initializeThread( exeNumberArray[i], i);
    }
    printf("Share: \n");
    for(int i=1; i<6; i++){
        printf("%d/%d\t",threadArray[i].exeNumber,total);
        if(i==5){
            printf("\n");
        }
    }
    for(int i=0; i<6; i++){
        printf("%d \t %d \t %d \t %d\n",threadArray[i].state, threadArray[i].countNumber, threadArray[i].exeNumber, threadArray[i].threadNumber);
    }
    printf("Threads:\n T1\tT2\tT3\tT4\tT5\n");
    bool isFinished = false;
    int sayma = 0;

    sleep(1);
    getcontext(&threadArray[0].context);
    signal(SIGALRM, handler);

    while(!isFinished){
        scheduler_one(0);
        //  printf("scheduler sonrası deneme: %d \n", sayma);
        sayma += 1;
        isFinished = isAllFinished();
    }
    printf("All threads are finished! \n");
}

*/
