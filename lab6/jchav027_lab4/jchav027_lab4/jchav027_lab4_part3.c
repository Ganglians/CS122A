/* Coder: Juan Chavez
 * Date:  10.18.2015
 * Class: CS122A 
 * Lab 6 Part 3
 * Description: Expand upon exercise 2 of the lab by adjusting the speed the illuminated LED shifts depending 
 * on how far left or right the joystick is tilted. The illuminated LED should shift once every 1000ms, 500ms, 
 * 250ms, or 100ms, depending on how far the joystick is tilted left or right
 */

/*#include <avr/io.h>
#include "scheduler.h"
#include "timer.h"

//Button detection
//#define BTN1 (~PINA & 0x01)
//#define BTN2 (~PINA & 0x02)

void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant\
	//     analog to digital conversions.
}
static unsigned char GCD_Period = 5;

unsigned char numTasks = 2;

task part1, part3;
task *taskList[] = {&part3, &part1};

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
		// Leftdate the value of how long will have passed until next check
		taskList[i]->elapsedTime += GCD_Period;
	}
}

unsigned int  joyc = 0;       // Keeps track of joystick
unsigned char C = 0x01;       // Pattern to display
unsigned char D = 0x01;		  // Rows enabled
static unsigned int speed = 0;       // Moving LED's speed tracker
static unsigned int pd_cntr = 0;     // Moving LED's speed based on period in GCD multiples

enum part3 {jwait, little_left, more_left, harder_left, hardest_left, little_right, more_right, harder_right, hardest_right, } jstate = -1;
int part3_Tick() {
	switch(jstate) { // transitions
		case -1:
			jstate = jwait;
		break;

		case jwait:
		    // Left detection
			if(joyc > 90 && joyc < 120) {
				jstate = little_left;
			}
			else if(joyc > 120 && joyc < 140) {
				jstate = more_left;
			}
			else if(joyc > 140 && joyc < 160) {
				jstate = harder_left;
			}
			else if(joyc > 160) {
				jstate = hardest_left;
			}
			
			// Right detection
			else if(joyc < 60 && joyc > 40) {
				jstate = little_right;
			}
			else if(joyc < 40 && joyc > 30) {
				jstate = more_right;
			}
			else if(joyc < 30 && joyc > 20) {
				jstate = harder_right;
			}
			else if(joyc < 20) {
				jstate = hardest_right;
			}
		break;
		
		case little_left:
			if(!(joyc > 90 && joyc < 120)) {
				jstate = jwait;
			}
		break;

		case more_left:
			if(!(joyc > 120 && joyc < 140)) {
				jstate = jwait;
			}
		break;

		case harder_left:
			if(!(joyc > 140 && joyc < 160)) {
				jstate = jwait;
			}
		break;

		case hardest_left:
			if(!(joyc > 160)) {
				jstate = jwait;
			}
		break;
		
		case little_right:
			if(!(joyc < 60 && joyc > 40)) {
				jstate = jwait;
			}			
		break;

		case more_right:
			if(!(joyc < 40 && joyc > 30)) {
				jstate = jwait;
			}
		break;

		case harder_right:
			if(!(joyc < 30 && joyc > 20)) {
				jstate = jwait;
			}
		break;

		case hardest_right:
			if(!(joyc < 20)) {
				jstate = jwait;
			}
		break;

		default:
			jstate = jwait;
		break;
	}

	switch(jstate) { // actions
		case -1:
			//Do nothing
		break;

		case jwait:
			//Do nothing
		break;
		
		case little_left:
			TimerOff();
			GCD_Period = 20;
			TimerSet(GCD_Period);
			TimerOn();
			part1.elapsedTime = part1.period;
			part1.period = GCD_Period;
			part3.elapsedTime = part3.period;
			part3.period = GCD_Period;
			//speed = 20;
		break;

		case more_left:
			TimerOff();
			GCD_Period = 10;
			TimerSet(GCD_Period);
			TimerOn();
			part1.elapsedTime = part1.period;
			part1.period = GCD_Period;
			part3.elapsedTime = part3.period;
			part3.period = GCD_Period;
		break;

		case harder_left:
			TimerOff();
			GCD_Period = 5;
			TimerSet(GCD_Period);
			TimerOn();
			part1.elapsedTime = part1.period;
			part1.period = GCD_Period;
			part3.elapsedTime = part3.period;
			part3.period = GCD_Period;
		break;

		case hardest_left:
			TimerOff();
			GCD_Period = 2;
			TimerSet(GCD_Period);
			TimerOn();
			part1.elapsedTime = part1.period;
			part1.period = GCD_Period;
			part3.elapsedTime = part3.period;
			part3.period = GCD_Period;
		break;

		case little_right:
			TimerOff();
			GCD_Period = 20;
			TimerSet(GCD_Period);
			TimerOn();
			part1.elapsedTime = part1.period;
			part1.period = GCD_Period;
			part3.elapsedTime = part3.period;
			part3.period = GCD_Period;
		break;

		case more_right:
			TimerOff();
			GCD_Period = 10;
			TimerSet(GCD_Period);
			TimerOn();
			part1.elapsedTime = part1.period;
			part1.period = GCD_Period;
			part3.elapsedTime = part3.period;
			part3.period = GCD_Period;
		break;

		case harder_right:
			TimerOff();
			GCD_Period = 5;
			TimerSet(GCD_Period);
			TimerOn();
			part1.elapsedTime = part1.period;
			part1.period = GCD_Period;
			part3.elapsedTime = part3.period;
			part3.period = GCD_Period;
		break;

		case hardest_right:
			TimerOff();
			GCD_Period = 2;
			TimerSet(GCD_Period);
			TimerOn();
			part1.elapsedTime = part1.period;
			part1.period = GCD_Period;
			part3.elapsedTime = part3.period;
			part3.period = GCD_Period;
		break;

		default:
			//Do nothing
		break;
	}
}


enum part1 {wait, left, hold_left, right, hold_right} state = -1;
int part1_Tick() {
	switch(state) { // Transitions
		case -1:
			pd_cntr = 0;
			state = wait;
		break;

		case wait:
			if(joyc > 90) {
				pd_cntr = 0;
				state = hold_left;
			}
			if(joyc < 60) {
				pd_cntr = 0;
				state = hold_right;
			}
		break;

		case left:
			state = wait;
		break;

		case hold_left:
			if(joyc < 90) {
				state = left;
			}
		break;

		case right:
			state = wait;
		break;

		case hold_right:
			if(joyc > 60) {
				state = right;
			}
		break;

		default:
			state = wait;
		break;
	}

	switch(state) { // Actions
		case -1:
			//Do nothing
		break;

		case wait:
			//Do nothing
		break;

		case left:
			//Make sure properly reset if stopped pressing joystick when bit disappears
			if(C < 0x01) {
				C = 0x01;
			}
			pd_cntr = 0;
		break;

		case hold_left:
			//++ pd_cntr;
			//if(pd_cntr == speed) {
				//pd_cntr = 0;

				if (C < 0x01) {
					C = 0x01;
				}
				else {
						C = C << 1;
				}
			//}
		break;

		case right:
			if (C < 0x01) {
				C = 0x80;
			}
			pd_cntr = 0;
		break;

		case hold_right:
			//++ pd_cntr;
			//if(pd_cntr == speed) {
				//pd_cntr = 0;
				if (C < 0x01) {
					C = 0x80;
				}	
				else {
					C = C >> 1;
				}			
			//}
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
	DDRA = 0x00; PORTA = 0xFF; // Set port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Set port C's 8 pins as ops
	DDRD = 0xFF; PORTD = 0x00; // Set port D's 8 pins as ops
	
	TimerSet(GCD_Period);
	TimerOn();
 
 	part1.elapsedTime = part1.period;
 	part1.period = GCD_Period;
 	part1.state = -1;
 	part1.TickFct = &part1_Tick;


 	part3.elapsedTime = part3.period;
 	part3.period = GCD_Period;
 	part3.state = -1;
 	part3.TickFct = &part3_Tick;
	 	
	A2D_init();	

    while(1) {
    	joyc = ADC; // Gets joystick data
		taskManage();
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
}*/