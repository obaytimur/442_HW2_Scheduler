//
// Created by ogi on 26.05.2022.
//

// Libraries that are needed for the homework initialized here
// I wanted to initialize them here because it is easier for me to add or remove
// libraries, main is too complicated
#include "ucontext.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include "signal.h"
#include <stdbool.h>
#include<time.h>

#ifndef HW2_THREADINFO_H
#define HW2_THREADINFO_H

// Thread information struct. It holds thread's context, variables, and flags.
struct ThreadInfo {
    ucontext_t context;     // Context of the thread
    int state ;             // State flag, state values are defined on the top of scheduler_lottery file
    int threadNumber;       // Thread's ID
    int countNumber;        // Counter that counts how many executions for cpu are done on this thread
    int exeNumber;          // Execution value for thread to run given as input
    int cpuBurstArray[3];   // Array holds cpu burst times
    int ioBurstArray[3];    // Array hold IO burst times
    int exeIndex;           // An index to flag which cpu burst is being done
    int ioIndex;            // An index to flag which IO burst is being done
    int ioCounter;          // Counter that counts how many executions for IO are done for this thread
};

#endif //HW2_THREADINFO_H
