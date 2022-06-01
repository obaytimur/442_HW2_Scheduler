#include "ThreadInfo.h"
// finished -1
// ready 0
// running 1
// empty 2
#define stackSize 4096

struct ThreadInfo threadArray[6];
bool isFinished = false;

void exitThread(int threadIndex) {
    free(threadArray[threadIndex].context.uc_stack.ss_sp);
    threadArray[threadIndex].state = -1;
}

void runThread(int threadIndex){
    threadArray[threadIndex].state = 1;
    threadArray[threadIndex].countNumber += 1;

    for(int i=0; i<threadIndex - 1; i++){
        printf("\t");
    }
    printf("%d, %d \n", threadArray[threadIndex].countNumber, threadArray[threadIndex].exeNumber);
    sleep(1);
    threadArray[threadIndex].countNumber += 1;

    for(int i=0; i<threadIndex - 1; i++){
        printf("\t");
    }
    printf("%d, %d \n", threadArray[threadIndex].countNumber, threadArray[threadIndex].exeNumber);
    threadArray[threadIndex].state = 0;

    if(threadArray[threadIndex].countNumber >= threadArray[threadIndex].exeNumber){
        exitThread(threadIndex);
    }
    sleep(1);
}

void initializeThread(int exeNumber, int threadIndex){

threadArray[threadIndex].state = 0;
threadArray[threadIndex].threadNumber= threadIndex;
threadArray[threadIndex].countNumber = 0;
threadArray[threadIndex].exeNumber = exeNumber;

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
}

bool isAllFinished(){
    for(int i=1; i<6; i++){
        if(threadArray[i].state != -1){
            return false;
        }
    }
    return true;
}

void scheduler_one(int initial){
    if (isAllFinished() ){
        isFinished = true;
    }
    signal(SIGALRM, scheduler_one);
    alarm(2);
    bool isFound = false;
    int randomNumber;
    while (!isFound) {
        randomNumber = rand() % 5 + 1;
        if (threadArray[randomNumber].state == 0 &&
            threadArray[randomNumber].countNumber < threadArray[randomNumber].exeNumber) {
            isFound = true;
        }
    }
    swapcontext(&threadArray[0].context, &threadArray[randomNumber].context);


}

int main(/* int argc, char **argv */){
    srand(time(NULL));
    int exeNumberArray[6] = {0,2,4,8,6,4};
//    exeNumberArray[0] = 0 ;
    int total = 20;
    /*
    for(int i=1; i<6; i++){
        exeNumberArray[i] = atoi(argv[i]);
        total = total + exeNumberArray[i];
    }
     */
    int threadIndex;
    for(int i=0; i<6; i++){
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
    sleep(1);
    printf("Threads:\n T1\tT2\tT3\tT4\tT5\n");

    sleep(1);
    getcontext(&threadArray[0].context);

    scheduler_one(0);
    while(!isFinished){
        if(isAllFinished()){
            break;
        }
    }
    printf("All threads are finished! \n");
    return 0;

}
