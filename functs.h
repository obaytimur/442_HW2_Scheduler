//
// Created by ogi on 30.05.2022.
//
#include "ThreadInfo.h"

#ifndef HW2_FUNCTS_H
#define HW2_FUNCTS_H

struct functs {

};

void initializeThread(struct ThreadInfo *threadArray[], int exeNumber, int threadIndex, int stackSize);
int createThread(struct ThreadInfo *threadArray[]);
void runThread(struct ThreadInfo *threadArray[], int threadIndex);
void exitThread(struct ThreadInfo *threadArray[], int threadIndex);
bool isAllFinished(struct ThreadInfo *threadArray[]);

#endif //HW2_FUNCTS_H
