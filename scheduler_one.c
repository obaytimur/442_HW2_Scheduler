#include "functs.h"
#include "signal.h"
#include <stdbool.h>

#define finished -1
#define ready 0
#define running 1
#define empty 2
#define stackSize 4096

struct ThreadInfo threadArray[6];

void scheduler_one(int signalInput){
    signal(SIGALRM, scheduler_one);
    alarm(2);
    bool isFound = false;
    int randomNumber;
    while(!isFound){
        randomNumber = rand()%4 +1;
        if(threadArray[randomNumber].countNumber < threadArray[randomNumber].exeNumber){
            isFound = true;
        }
    }
    swapcontext(&threadArray[randomNumber].context, &threadArray[0].context);
}

int main(int argc, char* argv[]){
    int exeNumberArray[5];
    int total = 0;
    for(int i=0; i<5; i++){
        exeNumberArray[i] = atoi(argv[i+1]);
        total = total + exeNumberArray[i];
    }
    for(int i=1; i<6; i++){
        int threadIndex;
        threadIndex = createThread((struct ThreadInfo *) &threadArray);
        if(threadIndex != 0){
            initializeThread((struct ThreadInfo *) &threadArray, exeNumberArray[i - 1], threadIndex, stackSize);
        }
    }
    printf("Share: \n");
    for(int i=0; i<5; i++){
        printf("T%d\t",i+1);
        if(i==4){
            printf("\n");
        }
    }
}

