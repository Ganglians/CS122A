/* Coder: Juan Chavez
 * Date:  10.14.2015
 * Class: CS122A 
 * Lab 5 Part 1
 * Description: Design a system where an illuminated row of the LED matrix can be shifted up or down based on button presses. 
 */

#include <avr/io.h>
#include "scheduler.h"
#include "timer.h"

//Button detection
#define BTN1 (~PINA & 0x01)
#define BTN2 (~PINA & 0x02)

static unsigned char GCD_Period = 10;

unsigned char numTasks = 1;

task part1;
task *taskList[] = {&part1};

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

unsigned char C = 0xFF;       //Pattern to display
unsigned char D = 0x02;		  //Rows enabled
enum part1 {wait, up, hold_up, down, hold_down} state = -1;
int part1_Tick() {
	switch(state) {//transitions
		case -1:
			state = wait;
		break;

		case wait:
			if(BTN1) {
				state = hold_up;
			}
			if(BTN2) {
				state = hold_down;
			}
		break;

		case up:
			state = wait;
		break;

		case hold_up:
			if(!BTN1) {
				state = up;
			}
		break;

		case down:
			state = wait;
		break;

		case hold_down:
			if(!BTN2) {
				state = down;
			}
		break;

		default:
			state = wait;
		break;
	}

	switch(state) {//actions
		case -1:
			//Do nothing
		break;

		case wait:
			//Do nothing
		break;

		case up:
			if(D < 0x10) {
				D = D << 1;	
			}
		break;

		case hold_up:
			//Do nothing
		break;

		case down:
			if(D > 0x01) {
				D = D >> 1;	
			}
		break;

		case hold_down:
			//Do nothing
		break;

		default:
			//Do nothing
		break;
	}
	PORTC = C;
	PORTD = ~D;
	//return state;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //Set port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; //Set port C's 8 pins as ops
	DDRD = 0xFF; PORTD = 0x00; //Set port D's 8 pins as ops
	
	TimerSet(GCD_Period);
	TimerOn();
 
 	part1.elapsedTime = part1.period;
 	part1.period = GCD_Period;
 	part1.state = -1;
 	part1.TickFct = &part1_Tick;
	 
	//op = 0xFF;
	
    while(1) {
		taskManage();
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
}