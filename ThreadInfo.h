//
// Created by ogi on 26.05.2022.
//

// Libraries that are neeeded for the homework initialized
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
    int countNumber;        // Counter that counts how many executions are done on this thread
    int exeNumber;          // Execution value for thread to run given as input
    int cpuBurstArray[3];
    int ioBurstArray[3];
    int exeIndex;
    int ioIndex;
    int ioCounter;
};

#endif //HW2_THREADINFO_H
