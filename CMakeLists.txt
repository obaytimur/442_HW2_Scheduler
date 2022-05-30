cmake_minimum_required(VERSION 3.22)
project(HW2 C)

set(CMAKE_C_STANDARD 99)

add_executable(HW2 scheduler_one.c functs.h functs.c ThreadInfo.c ThreadInfo.h)
