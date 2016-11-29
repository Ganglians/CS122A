/* Name:  	 Juan Chavez
 * Email: 	 jchav027@ucr.edu
 * Date:  	 10.25.2015
 * Class:    CS122A
 * Homework 2 Part 3
 * Description: 
 */

#include "rims.h"

// Numer that is sent (for testing purposes)
#define VAL 39;

// Struct for Tasks represent a running process in our simple real-time operating system
typedef struct _task {
	// Tasks should have members that include: state, period,
	// a measurement of elapsed time, and a function pointer.
	signed 	 char state; 		// Task's current state
	unsigned long period; 		// Task period
	unsigned long elapsedTime; 	// Time elapsed since last task tick
	int (*TickFct)(int); 		// Task tick function
} task;

unsigned char numTasks = 1;
unsigned long GCD_Period = 300;
task SPI;
task *taskList[] = { &SPI };

void taskManage() {
	unsigned char i = 0;
	for(i = 0; i < numTasks; i++) {
		// Task is ready to tick
		if ( taskList[i]->elapsedTime == taskList[i]->period ) {
			// Setting next state for task
			taskList[i]->state = taskList[i]->TickFct(taskList[i]->state);
			// Reset the elapsed time for next tick.
			taskList[i]->elapsedTime = 0;
		}
		// Update the value of how long will have passed until next check
		taskList[i]->elapsedTime += GCD_Period;		
	}
}

unsigned char buff = VAL;
enum SPI_states {INIT, IDLE, ACTIVE} SPI_state = INIT;

int SPI_Tick(int s) {
	switch(SPI_state) { // Actions
        
		case INIT:
			B0 = 0;
		break;

		case IDLE:
			// Do nothing
		break;

		case ACTIVE:
		     if(!B0) { // Oscillate SClock
		     	B0 = 0x01;
		     }
		     else {
		     	B0 = 0x00;
		     }
		     
			//Send rightmost bit
			if((buff & 0x80) == 0) {
				B1 = 0;
			}
			else {
				B1 = 1;
			}
			//Shift buff one to left
			buff <<= 1;

			//Receive bit
			if(A1 == 0) { // Set rightmost bit to zero
				buff = buff & 0xFE;
			}
			else {
				buff = (buff | 0x01); // Else set it to one
			}
		break;

		default:
			// Do nothing
		break;
	}

	switch(SPI_state) { // Transitions 
		case INIT:
			SPI_state = IDLE;
		break;

		case IDLE:
			if(A0) {
				// Set SS bit to active low
				B0 = 0;				
				SPI_state = ACTIVE;
			}
		break;

		case ACTIVE:
			if(!A0) {
				SPI_state = INIT;
			}
		break;

		default:
			
		break;
	}
    //return SPI_state;
}

void main() {
	TimerSet(GCD_Period);
	TimerOn();

 	SPI.elapsedTime = SPI.period;
 	SPI.period = GCD_Period;
 	SPI.state = SPI_state;
 	SPI.TickFct = &SPI_Tick;
	
    while (1) { 
      taskManage();
    }
}