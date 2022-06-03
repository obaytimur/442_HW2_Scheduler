#include "ThreadInfo.h"
// finished -1
// ready 0
// running 1
// empty 2
// IO 3
// Last Exe 4
// Last IO 5
#define stackSize 16384 // Given in homework parameter

struct ThreadInfo threadArray[6]; // Array of threadInfo objects initialized
bool isFinished = false;            // A bool variable to flag the end of the execution

// I wanted to write functions in order which is given in the pdf, however, some functions were needed to be initialized
// first to be used. For example, exitThread must be written before runThread because it is used in runThread.
// Moreover, runThread must be written before initializeThread since initialization requires runThread function to use
// in makecontext().

void printTerminal(){
    printf("Running>");
    for(int i=1; i<6; i++){
        if(threadArray[i].state == 1){
            printf("T%d ",threadArray[i].threadNumber);
        }
    }
    printf("\t \t Ready>");
    for(int i=1; i<6; i++){
        if(threadArray[i].state == 0){
            printf("T%d ",threadArray[i].threadNumber);
        }
    }
    printf("\t \t Finished>");
    for(int i=1; i<6; i++){
        if(threadArray[i].state == -1){
            printf("T%d ",threadArray[i].threadNumber);
        }
    }
    printf("\t \t IO>");
    for(int i=1; i<6; i++){
        if(threadArray[i].state == 3){
            printf("T%d ",threadArray[i].threadNumber);
        }
    }
    printf("\n");
}

void exitThread(int threadIndex) {  // Function for thread to exit
    threadArray[threadIndex].state = -1;    // Then it sets the state of the object to finished
    //free(threadArray[threadIndex].context.uc_stack.ss_sp);
}

void runThread(int threadIndex){    // Function that runs the threads

    if(threadArray[threadIndex].state == 0){
        threadArray[threadIndex].state = 1;     // Starts by setting flag to running
        threadArray[threadIndex].countNumber += 1;  // Increase counter one as one execution is done
    }
    printTerminal();
    if(threadArray[threadIndex].state ==1){
        for(int i=0; i<threadIndex - 1; i++){
            printf("\t");   // Prints enough tabs for the corresponding thread
        }
        // Prints the counter value of the thread
        printf("%d \n", threadArray[threadIndex].countNumber);
    }
    sleep(1); // Thread sleeps for one second to represent the operation time

    for(int i=1; i<6; i++){
        if(threadArray[i].state == 3){
            threadArray[i].ioCounter += 1;
        }
    }
    if(threadArray[threadIndex].state == 1){
        threadArray[threadIndex].countNumber += 1; // The same operations are done for the second time,
        for(int i=0; i<threadIndex - 1; i++){
            printf("\t");
        }
        printf("%d \n", threadArray[threadIndex].countNumber);
        threadArray[threadIndex].state = 0; // At the end of the second execution, state is set to ready again since runs are done
    }
    for(int i=1; i<6; i++){
        if(threadArray[i].state == 3){
            threadArray[i].ioCounter += 1;
        }
    }
    // Here I assumed, every execution value which will be given by the user was going to be even number as it
    // was shown in the homework pdf. Therefore, even if the execution number is odd, e.g. 5, it will wait
    // for it to be the next even number, e.g. 6, then exits the thread. This if statement can also be put
    // above and made for the odd ones too.
    if(threadArray[threadIndex].countNumber >= threadArray[threadIndex].cpuBurstArray[threadArray[threadIndex].exeIndex]){
        threadArray[threadIndex].state = 3;
        threadArray[threadIndex].countNumber = 0;
        threadArray[threadIndex].exeIndex += 1;
    }

    for(int i=1; i<6; i++){
        if(threadArray[i].ioCounter >= threadArray[i].ioBurstArray[threadArray[i].ioIndex]){
            if (threadArray[i].ioIndex == 2){
                exitThread(i);
            }
            else{
                threadArray[i].state = 0;
                threadArray[i].ioCounter = 0;
                threadArray[i].ioIndex += 1;
            }
        }
    }
    sleep(1); // Again sleeps for one second representing execution
}
// Thread creation function, takes index for the thread which is going to be created.
int createThread(int threadIndex){
    int returnObject = getcontext(&threadArray[threadIndex].context); // A return object is used to get context of the thread
    if(returnObject == -1) {                                               // and if it is -1, it means that thread coult not created
        printf("Could not create thread object");                   // as it was described here:
    }                                                                      // https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-getcontext-get-user-context
}

// Thread initialization function
void initializeThread(int threadIndex, int* burstArray, int* ioArray){
threadArray[threadIndex].state = 0;                     // Sets state flag to zero since new initializations are going to be ready
threadArray[threadIndex].threadNumber= threadIndex;     // Thread ID is given as thread's index
threadArray[threadIndex].countNumber = 0;               // Counter start from 0, no executions are done yet
threadArray[threadIndex].exeIndex = 0;
threadArray[threadIndex].ioIndex = 0;
threadArray[threadIndex].ioCounter = 0;

if(threadIndex!=0){
    threadArray[threadIndex].exeNumber = burstArray[0] + burstArray[1] + burstArray[2];         // Input given by user, set as the total execution time
    threadArray[threadIndex].cpuBurstArray[0] = burstArray[0];
    threadArray[threadIndex].cpuBurstArray[1] = burstArray[1];
    threadArray[threadIndex].cpuBurstArray[2] = burstArray[2];
    threadArray[threadIndex].ioBurstArray[0] = ioArray[0];
    threadArray[threadIndex].ioBurstArray[1] = ioArray[1];
    threadArray[threadIndex].ioBurstArray[2] = ioArray[2];
}

// Before using makecontext function some constraints are made as it was given here:
// https://pubs.opengroup.org/onlinepubs/7908799/xsh/makecontext.html
threadArray[threadIndex].context.uc_link = &threadArray[0].context; // context is linked to the first object of the array
threadArray[threadIndex].context.uc_stack.ss_sp = malloc(stackSize); // memory allocation for stack size is done
threadArray[threadIndex].context.uc_stack.ss_size = stackSize; // size set
makecontext(&threadArray[threadIndex].context, (void (*)(void))runThread, 1, threadIndex);
// the function saves the desired context with the corresponding function to run when called with
// swapcontext or setcontext
}

// A function whose return is bool to flag whether the threads are finished or not
bool isAllFinished(){
    for(int i=1; i<6; i++){                 // searches in loop for all threads
        if(threadArray[i].state != -1){     // if there are any unfinished threads exit loop immediately with return
            return false;                   // value of false saying, not finished
        }
    }
    return true;
}

// Scheduler function with lottery selection
void scheduler_lottery(int initial){
    signal(SIGALRM, scheduler_lottery); // Signal is created with handling function is the scheduler itself
    alarm(2);       // At every 2 seconds, it interrupts the system and handles it
    bool isFound = false;   // bool variable used to flag whether there is a suitable thread to be used in lottery selection
    int randomNumber;
    while (!isFound) {
        randomNumber = rand() % 5 + 1;                  // Since threads are in the array with indexes 1-5, a random number is
        if (threadArray[randomNumber].state == 0 &&     // generated with mod 5, if that thread is suitable, then random number
            threadArray[randomNumber].countNumber < threadArray[randomNumber].cpuBurstArray[threadArray[randomNumber].exeIndex]) {  // can be used for the swapcontext().
            isFound = true;
        }
        if(threadArray[0].state != 0 && threadArray[1].state != 0 && threadArray[2].state != 0 &&
                threadArray[3].state != 0 && threadArray[4].state != 0){
            for(int i=1; i<6; i++){
                if(threadArray[i].state == 3){
                    randomNumber = i;
                    isFound = true;
                    break;
                }
            }
        }
    }
    swapcontext(&threadArray[0].context, &threadArray[randomNumber].context);   // swapcontext calls the function which
    // is defined in initialization for the thread, this part runs the runThread
}

void scheduler_SRTF(int initial){
    signal(SIGALRM,scheduler_SRTF);
    alarm(2);
    int shortestRemExeInd = 0;
    int exeValue = 999;
    for(int i=1; i<6; i++){
        if(/*threadArray[i].state != -1 && threadArray[i].state != 3 */ threadArray[i].state==0 && threadArray[i].cpuBurstArray[threadArray[i].exeIndex] < exeValue){
            exeValue = threadArray[i].cpuBurstArray[threadArray[i].exeIndex];
            shortestRemExeInd = i;
        }
    }
    if(shortestRemExeInd == 0){
        for(int i=1; i<6; i++){
            if(threadArray[i].state == 3){
                shortestRemExeInd = i;
                break;
            }
        }
    }
    if (shortestRemExeInd == 0){
        printf("All threads are finished! \n");
        for(int i=1; i<6; i++){
            free(threadArray[i].context.uc_stack.ss_sp);
        }
        exit(-1);
    }
    swapcontext(&threadArray[0].context, &threadArray[shortestRemExeInd].context);   // swapcontext calls the function which
    // is defined in initialization for the thread, this part runs the runThread
    if (isAllFinished() ) {      // Then it is checked whether all the threads are done or not
        isFinished = true;
    }
}

int main(){      // main takes input values for the threads from the txt file
    FILE *fileObject ;
    fileObject= fopen("/home/ogi/Media/ODTÜ EE/4-2/442/HW2/input.txt","r");
    int exeNumberArray[31];
    if(fileObject==NULL) {
        printf("ERROR: Could not open file: input.txt");
        return 1;
    }
    int tempObject;
    while(!feof(fileObject)) {
        for (int i = 0; i < 31; i++) {
            fscanf(fileObject, "%d", &tempObject);
            exeNumberArray[i] = tempObject;
        }
    }
    fclose(fileObject);

    srand(time(NULL));      // srand function is needed for the lottery selection, if not used lottery selection
    // were following the same sequence

    createThread(0);
    initializeThread(0,0,0);

    int total = 0;
    int cpuBurstArray[5][3];
    int ioBurstArray[5][3];
    for(int i=0; i<5;i++){
        for(int j=0; j<3; j++){
            cpuBurstArray[i][j] = exeNumberArray[3*i + j];
            total += exeNumberArray[3*i + j];
        }
    }
    for(int i=0; i<5;i++){
        for(int j=0; j<3; j++){
            ioBurstArray[i][j] = exeNumberArray[3*i + j +15];
        }
    }
    int indexNumber=0;
    for(int i=0; i<5; i++){     // threads are created and initialized by calling the functions
        createThread(i+1);
        initializeThread(i+1, cpuBurstArray[i], ioBurstArray[i]);
    }
    if(exeNumberArray[30] == 0){
        printf("Scheduler is selected as Lottery. \n");
    }
    else if (exeNumberArray[30] == 1){
        printf("Scheduler is selected as SRTF. \n");
    }
    // From here to getcontext line, main trivially prints writings
    printf("Share: \n");
    for(int i=1; i<6; i++){
        printf("%d/%d\t",threadArray[i].exeNumber,total);
        if(i==5){
            printf("\n");
        }
    }
    sleep(1);
    printf("Threads:\n T1\tT2\tT3\tT4\tT5\n");

    sleep(1);
    getcontext(&threadArray[0].context);    // Main thread's context is called to be used in the functions

    if(exeNumberArray[30] == 0){
        scheduler_lottery(0);   // scheduler is called, since it is self calling function, until stopped it will run indefinitely
        // with the times of the alarm
    }
    else if (exeNumberArray[30] == 1){
        scheduler_SRTF(0);
    }
    while(!isFinished){
    }
    return 0;
}
