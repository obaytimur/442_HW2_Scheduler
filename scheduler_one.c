#include "ThreadInfo.h"
// finished -1
// ready 0
// running 1
// empty 2
#define stackSize 4096 // Given in homework parameter

struct ThreadInfo threadArray[6]; // Array of threadInfo objects initialized
bool isFinished = false;            // A bool variable to flag the end of the execution

// I wanted to write functions in order which is given in the pdf, however, some functions were needed to be initialized
// first to be used. For example, exitThread must be written before runThread because it is used in runThread.
// Moreover, runThread must be written before initializeThread since initialization requires runThread function to use
// in makecontext().
void exitThread(int threadIndex) {  // Function for thread to exit
    free(threadArray[threadIndex].context.uc_stack.ss_sp);  // It, firstly, frees the memory of the object
    threadArray[threadIndex].state = -1;    // Then it sets the state of the object to finished
}

void runThread(int threadIndex){    // Function that runs the threads
    threadArray[threadIndex].state = 1;     // Starts by setting flag to running
    threadArray[threadIndex].countNumber += 1;  // Increase counter one as one execution is done

    for(int i=0; i<threadIndex - 1; i++){
        printf("\t");   // Prints enough tabs for the corresponding thread
    }
    // Prints the counter value of the thread
    printf("%d, %d \n", threadArray[threadIndex].countNumber, threadArray[threadIndex].exeNumber);
    sleep(1); // Thread sleeps for one second to represent the operation time
    threadArray[threadIndex].countNumber += 1; // The same operations are done for the second time,

    for(int i=0; i<threadIndex - 1; i++){
        printf("\t");
    }
    printf("%d, %d \n", threadArray[threadIndex].countNumber, threadArray[threadIndex].exeNumber);
    threadArray[threadIndex].state = 0; // At the end of the second execution, state is set to ready again since runs are done

    // Here I assumed, every execution value which will be given by the user was going to be even number as it
    // was shown in the homework pdf. Therefore, even if the execution number is odd, e.g. 5, it will wait
    // for it to be the next even number, e.g. 6, then exits the thread. This if statement can also be put
    // above and made for the odd ones too.
    if(threadArray[threadIndex].countNumber >= threadArray[threadIndex].exeNumber){
        exitThread(threadIndex);
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
void initializeThread(int exeNumber, int threadIndex){
threadArray[threadIndex].state = 0;                     // Sets state flag to zero since new initializations are going to be ready
threadArray[threadIndex].threadNumber= threadIndex;     // Thread ID is given as thread's index
threadArray[threadIndex].countNumber = 0;               // Counter start from 0, no executions are done yet
threadArray[threadIndex].exeNumber = exeNumber;         // Input given by user, set as the total execution time

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
            threadArray[randomNumber].countNumber < threadArray[randomNumber].exeNumber) {  // can be used for the swapcontext().
            isFound = true;
        }
    }
    swapcontext(&threadArray[0].context, &threadArray[randomNumber].context);   // swapcontext calls the function which
    // is defined in initialization for the thread, this part runs the runThread
    if (isAllFinished() ){      // Then it is checked whether all the threads are done or not
        isFinished = true;
    }
}

int main(/* int argc, char **argv */){      // main takes input values for the threads
    srand(time(NULL));      // srand function is needed for the lottery selection, if not used lottery selection
    // were following the same sequence
    int exeNumberArray[6] = {0,2,4,8,6,4};  // Arrays hold the execution values
//    exeNumberArray[0] = 0 ;
    int total = 20;
    /*
    for(int i=1; i<6; i++){
        exeNumberArray[i] = atoi(argv[i]);
        total = total + exeNumberArray[i];
    }
     */
    int threadIndex;
    for(int i=0; i<6; i++){     // threads are created and initialized by calling the functions
        createThread(i);
        initializeThread( exeNumberArray[i], i);
    }
    // From here to getcontext line, main trivially prints writings
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
    getcontext(&threadArray[0].context);    // Main thread's context is called to be used in the functions

    scheduler_lottery(0);   // scheduler is called, since it is self calling function, until stopped it will run indefinitely
                                  // with the times of the alarm
    while(!isFinished){
    }
    printf("All threads are finished! \n");
    return 0;

}
