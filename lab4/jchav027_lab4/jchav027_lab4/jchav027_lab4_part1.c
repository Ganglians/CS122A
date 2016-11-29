/* Coder: Juan Chavez
 * Date:  10.10.2015
 * Class: CS122A 
 * Lab 4 Part 1
 * Description: Using a shift register to add/subtract from a value and display it's binary representation on the LED's.
 */

#include <avr/io.h>
#include "scheduler.h"
#include "timer.h"

unsigned char GCD_Period = 5;

void transmit_data(unsigned char data) {
	int i;
	for (i = 7; i >= 0 ; --i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x04;
	}
	// set RCLK = 1. Rising edge copies data from the “Shift” register to the “Storage” register
	PORTC |= 0x02;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
}

unsigned char numTasks = 1;

task part1;
task *taskList[] = {&part1};
enum part1Ticks {start, wait, add, sub, reset, waitRelease} part1State = -1;

unsigned char op = 0xFF;

#define BTN1 (~PINA & 0x01)
#define BTN2 (~PINA & 0x02)
int part1TickFunction(int state)
{
	int tickState = state;

	switch(tickState) //transitions
	{
		case -1:
			tickState = start;
		break;
		
		case start:
			tickState = wait;
		break;
		
		case wait:
			if((BTN1 != 0) && (BTN2 != 0)) {
				tickState = reset;
			}
			else if((BTN1 != 0) && (BTN2 == 0)) {
				tickState = add;
			}
			else if((BTN1 == 0) && (BTN2 != 0)) {
				tickState = sub;
			}
			else {
				tickState = wait;
			}
		break;
		
		case add:
			if((BTN1 != 0) && (BTN2 != 0)) {
				tickState = reset;
			}
			else {
				tickState = waitRelease;
			}
		break;
		
		case sub:
			if((BTN1 != 0) && (BTN2 != 0)) {
				tickState = reset;
			}
			else {
				tickState = waitRelease;
			}
		break;
		
		case reset:
			if((BTN1 != 0) && (BTN2 != 0)) {
				tickState = reset;
			}
			else {
				tickState = waitRelease;
			}
		break;
		
		case waitRelease:
			if((BTN1 != 0) && (BTN2 != 0)) {
				tickState = reset;
			}
			else if((BTN1 == 0) && (BTN2 == 0)) {
				tickState = wait;
			}
			else {
				tickState = waitRelease;
			}
		break;
		
		default:
			tickState = reset;
		break;
	}

	switch(tickState) //actions
	{
		case -1:
		break;
		
		case start:
			op = 0;
		break;
		
		case wait:
			transmit_data(op);
		break;
		
		case add:
			if(op < 255) { op++;
		}

		break;
		
		case sub:
			if(op > 0) {
				op--;
			}
		break;
		
		case reset:
			op = 0;
		break;
		
		case waitRelease:
			//Do nothing
		break;
		
		default:
			//Do nothing
		break;
	}	
	return tickState;
}

void taskManage() {
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
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //Set port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; //Set port C's 8 pins as ops
	
	TimerSet(GCD_Period);
	TimerOn();
 
 	part1.elapsedTime = part1.period;
 	part1.period = GCD_Period;
 	part1.state = -1;
 	part1.TickFct = &part1TickFunction;
	 
	//op = 0xFF;
	
    while(1) {
		taskManage();
    }
}