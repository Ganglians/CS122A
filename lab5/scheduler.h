// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef SCHEDULER_H
#define SCHEDULER_H

////////////////////////////////////////////////////////////////////////////////
//Functionality - finds the greatest common divisor of two values
//Parameter: Two long int's to find their GCD
//Returns: GCD else 0
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a % b;
		if( c == 0 ) { return b; }
		a = b;
		b = c;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
//Struct for Tasks represent a running process in our simple real-time operating system
typedef struct _task{
	// Tasks should have members that include: state, period,
	//a measurement of elapsed time, and a function pointer.
	signed 	 char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;


/*
unsigned char numTasks;
unsigned long GCD_Period;
task sm500, sm1000, sm2500;
task *taskList[];// = {&sm500, &sm1000, &sm2500};
void taskManage()
{
	unsigned char i = 0;
	for(i = 0; i < numTasks; i++)
	{
		// Task is ready to tick
		if ( taskList[i]->elapsedTime == taskList[i]->period ) {
			// Setting next state for task
			taskList[i]->state = taskList[i]->TickFct(taskList[i]->state);
			// Reset the elapsed time for next tick.
			taskList[i]->elapsedTime = 0;
		}
		//Update the value of how long will have passed until next check
		taskList[i]->elapsedTime += GCD_Period;		
	}
}*/

#endif //SCHEDULER_H