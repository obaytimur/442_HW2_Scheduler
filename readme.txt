// This project is done by Fazlı Oğulcan Baytimur, 2231389.
// All other necessary files or previos versions of the project
// can be found at https://github.com/obaytimur/442_HW2_Scheduler


Welcome to the scheculer project! 


This program takes input from "input.txt" file. Thus, please do not change its name.
There is also an execution file in the folder, which can be called from terminal. 
Remember that scheduler type is defined in the input file therefore, calling from 
terminal always results the same unless input file is updated.


The project is very straight forward. It assigns the inputs to according threads' cpu
and IO arrays. The following format should be followed when an input is desired to be
given:

1. "input.txt" file should always have 31 integers.
2. The first 15 number is for the cpu bursts, with ascending thread order from 1 to 5
3. The second 15 number is for the IO bursts, with also ascending thread order from 1 to 5
4. The last number is for the scheduler type: 0 for lottery, and 1 for the SRTF

An example input can be: "2 2 2 4 4 4 6 6 6 8 8 8 10 10 10 2 2 2 4 4 4 6 6 6 8 8 8 10 10 10 0"
			   T1EX, T2EX, T3EX, T4EX,  T5EX,  T1IO,  T2IO, T3IO, T4IO,   T5IO, Scheduler Type
			   
			   
At the end of the run, you should take "Process finished with exit code 255" if you are running 
the program in an editor after seeing "All threads are finished!". The reason is, I used 
exit(-1) command to exit code. This is not an error, as it is intended. 

