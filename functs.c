//
// Created by ogi on 30.05.2022.
//
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