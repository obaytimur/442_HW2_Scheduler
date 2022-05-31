//
// Created by ogi on 30.05.2022.
//

#include "ucontext.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include "signal.h"
#include <stdbool.h>
#include<time.h>

#ifndef HW2_THREADINFO_H
#define HW2_THREADINFO_H

struct ThreadInfo {
    ucontext_t context;
    int state ;
    int threadNumber;
    int countNumber;
    int exeNumber;
};

#endif //HW2_THREADINFO_H
