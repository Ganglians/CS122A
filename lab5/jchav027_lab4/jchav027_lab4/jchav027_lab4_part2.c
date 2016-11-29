/* Coder: Juan Chavez
 * Date:  10.14.2015
 * Class: CS122A 
 * Lab 5 Part 1
 * Description: Design a system where an illuminated row of the LED matrix can be shifted left or right based on button presses. 
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
		//leftdate the value of how long will have passed until next check
		taskList[i]->elapsedTime += GCD_Period;
	}
}

//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
enum Demo_States {shift};
int Demo_Tick(int state) {

	// Local Variables
	static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xFE;  	// Row(s) displaying pattern.
	// 0: display pattern on row
	// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case shift:	break;
		default:	state = shift;
		break;
	}
	
	// Actions
	switch (state) {
		case shift:	if (row == 0xEF && pattern == 0x01) { // Reset demo
			pattern = 0x80;
			row = 0xFE;
		}
		else if (pattern == 0x01) { // Move LED to start of next row
			pattern = 0x80;
			row = (row << 1) | 0x01;
		}
		else { // Shift LED one spot to the right on current row
			pattern >>= 1;
		}
		break;
		default:	break;
	}
	
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern
	
	return state;
}

unsigned char C = 0x01;       //Pattern to display
unsigned char D = 0x01F;		  //Rows enabled
enum part1 {wait, left, hold_left, right, hold_right} state = -1;
int part1_Tick() {
	switch(state) {//transitions
		case -1:
			state = wait;
		break;

		case wait:
			if(BTN1) {
				state = hold_left;
			}
			if(BTN2) {
				state = hold_right;
			}
		break;

		case left:
			state = wait;
		break;

		case hold_left:
			if(!BTN1) {
				state = left;
			}
		break;

		case right:
			state = wait;
		break;

		case hold_right:
			if(!BTN2) {
				state = right;
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

		case left:
			if(C > 0x01) {
				C = C >> 1;	
			}
		break;

		case hold_left:
			//Do nothing
		break;

		case right:
			if(C < 0x80) {
				C = C << 1;	
			}
		break;

		case hold_right:
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