/* Coder: Juan Chavez
 * Date:  10.14.2015
 * Class: CS122A 
 * Lab 5 Part 1
 * Description: Design a system where a 3x4 hollow rectangle is displayed in the center of the LED matrix 
 */

#include <avr/io.h>
#include "scheduler.h"
#include "timer.h"

//Button detection
#define BTN1 (~PINA & 0x01)
#define BTN2 (~PINA & 0x02)
#define BTN3 (~PINA & 0x04)
#define BTN4 (~PINA & 0x08)

static unsigned char GCD_Period = 1;

unsigned char numTasks = 2;

task task1, task2;
task *taskList[] = {&task1, &task2};

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

//s1: Two colums that make up the square
unsigned char C1 = 0x24;       //Pattern to display
unsigned char D1 = 0x0E;		  //Rows enabled
enum s1 {wait1, left1, hold_left1, right1, hold_right1, up1, hold_up1, down1, hold_down1} state1 = -1;
int s1_Tick() {
	switch(state1) {//transitions
		case -1:
			state1 = wait1;
		break;

		case wait1:
			if(BTN1) {
				state1 = hold_left1;
			}
			if(BTN2) {
				state1 = hold_right1;
			}
			if(BTN3) {
				state1 = hold_up1;
			}
			if(BTN4) {
				state1 = hold_down1;
			}
		break;

		case left1:
			state1 = wait1;
		break;

		case hold_left1:
			if(!BTN1) {
				state1 = left1;
			}
		break;

		case right1:
			state1 = wait1;
		break;

		case hold_right1:
			if(!BTN2) {
				state1 = right1;
			}
		break;

		case up1:
			state1 = wait1;
		break;

		case hold_up1:
			if(!BTN3) {
				state1 = up1;
			}
		break;

		case down1:
			state1 = wait1;
		break;

		case hold_down1:
			if(!BTN4) {
				state1 = down1;
			}
		break;

		default:
			state1 = wait1;
		break;
	}

	switch(state1) {//actions
		case -1:
			//Do nothing
		break;

		case wait1:
			//Do nothing
		break;

		case left1:
			if(C1 > 0x09) {
				C1 = C1 >> 1;	
			}
		break;

		case hold_left1:
			//Do nothing
		break;

		case right1:
			if(C1 < 0x90) {
				C1 = C1 << 1;	
			}
		break;

		case hold_right1:
			//Do nothing
		break;

		case up1:
			if(D1 < 0x1C) {
				D1 = D1 << 1;
			}
		break;

		case hold_up1:
			//Do nothing
		break;

		case down1:
		
			if(D1 > 0x07) {
				D1 = D1 >> 1;
			}
		break;

		case hold_down1:
			//Do nothing
		break;

		default:
			//Do nothing
		break;
	}
	PORTC = C1;
	PORTD = ~D1;
	//return state1;
}

unsigned char C2 = 0x18;       //Pattern to display
unsigned char D2 = 0x0A;		  //Rows enabled
enum s2 {wait2, left2, hold_left2, right2, hold_right2, up2, hold_up2, down2, hold_down2} state2 = -1;
int s2_Tick() {
	switch(state2) {//transitions
		case -1:
			state2 = wait2;
		break;

		case wait2:
			if(BTN1) {
				state2 = hold_left2;
			}
			if(BTN2) {
				state2 = hold_right2;
			}
			if(BTN3) {
				state2 = hold_up2;
			}
			if(BTN4) {
				state2 = hold_down2;
			}
		break;

		case left2:
			state2 = wait2;
		break;

		case hold_left2:
			if(!BTN1) {
				state2 = left2;
			}
		break;

		case right2:
			state2 = wait2;
		break;

		case hold_right2:
			if(!BTN2) {
				state2 = right2;
			}
		break;

		case up2:
			state2 = wait2;			
		break;

		case hold_up2:
			if(!BTN3) {
				state2 = up2;
			}
		break;

		case down2:
			state2 = wait2;
		break;

		case hold_down2:
			if(!BTN4) {
				state2 = down2;
			}
		break;

		default:
			state2 = wait2;
		break;
	}

	switch(state2) {//actions
		case -1:
			//Do nothing
		break;

		case wait2:
			//Do nothing
		break;

		case left2:
			if(C2 > 0x06) {
				C2 = C2 >> 1;	
			}
		break;

		case hold_left2:
			//Do nothing
		break;

		case right2:
			if(C2 < 0x60) {
				C2 = C2 << 1;	
			}
		break;

		case hold_right2:
			//Do nothing
		break;

		case up2:
			if(D2 < 0x14) {
				D2 = D2 << 1;
			}
		break;

		case hold_up2:
			//Do nothing
		break;

		case down2:
		
			if(D2 > 0x05) {
				D2 = D2 >> 1;
			}
		break;

		case hold_down2:
			//Do nothing
		break;

		default:
			//Do nothing
		break;
	}
	PORTC = C2;
	PORTD = ~D2;
	//return state2;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //Set port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; //Set port C's 8 pins as ops
	DDRD = 0xFF; PORTD = 0x00; //Set port D's 8 pins as ops
	
	TimerSet(GCD_Period);
	TimerOn();
 
 	task1.elapsedTime = task1.period;
 	task1.period = 1;
 	task1.state = -1;
 	task1.TickFct = &s1_Tick;

 	task2.elapsedTime = task2.period;
 	task2.period = 2;
 	task2.state = -1;
 	task2.TickFct = &s2_Tick;	 
	//op = 0xFF;
	
    while(1) {
		taskManage();
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
}