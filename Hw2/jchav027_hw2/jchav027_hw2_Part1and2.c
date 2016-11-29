/* Name:  	 Juan Chavez
 * Email: 	 jchav027@ucr.edu
 * Date:  	 10.25.2015
 * Class:    CS122A
 * Homework 2 Parts 1 and 2
 * Description: UART
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
task UART;
task *taskList[] = { &UART };

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

unsigned char i;
unsigned char buff;
unsigned char par; 
enum UART_states {INIT, IDLE, TRANSMIT, PARITY} UART_state = INIT;

int UART_Tick(int s) {
	switch(UART_state) { // Actions
		case INIT:
			// Set the START/STOP bit to 1, this means state machine is idle
			B0 = 1;
            // Make sure not outputting anything 
            B1 = 0;
			// Set buff to val that will be sent (39 in this case)
			buff = VAL;
			// Set the bit counter to 0 (because no bits have been transmitted yet)
			i = 0;
            // Set parity bit to 0
            par = 0;
		break;

		case IDLE:
			// Do nothing
		break;

		case TRANSMIT:
			// Transmit the bit on pin B1
			B1 = buff & 0x01;
			// Bit shift once to the right
			buff = buff >> 1;
			// Add to the counter
			i ++;
            // Add to parity counter if bit is 1
            if(B1) {
                par ++;    
            }
		break;

        case PARITY:
           if(!(par % 2)) {
               B1 = 1;
           }
           else {
               B1 = 0;
           }
        break;
        
		default:
			//Do nothing
		break;
	}

	switch(UART_state) { // Transitions 
		case INIT:
			// Move to the idle state
			UART_state = IDLE;
		break;

		case IDLE:
			if (A0) {
                B0 = 0;
				UART_state = TRANSMIT;
			}
		break;

		case TRANSMIT:
			// Deliver final bit after shifted
			if(i > 8) {
				UART_state = PARITY;
			}
		break;
        
        case PARITY:
            // Reset after sending the parity
            UART_state = INIT;
        break;

		default:
			UART_state = INIT;
		break;
	}
    //return UART_state;
}

void main() {
	TimerSet(GCD_Period);
	TimerOn();

 	UART.elapsedTime = UART.period;
 	UART.period = GCD_Period;
 	UART.state = UART_state;
 	UART.TickFct = &UART_Tick;
	
    while (1) { 
      taskManage();
    }
}