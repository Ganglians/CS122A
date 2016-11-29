/* Coder: Juan Chavez
 * Date:  10.17.2015
 * Class: CS122A 
 * Lab 6 Part 4
 * Description: Using the A2D result stored in ADC, design a system where an illuminated LED is shifted 
 * left or right along the top row an LED matrix. 
 */

#include <avr/io.h>
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

	// Pins on PORTA are used as input for A2D conversion
	//    The default channel is 0 (PA0)
	// The value of pinNum determines the pin on PORTA
	//    used for A2D conversion
	// Valid values range between 0 and 7, where the value
	//    represents the desired pin for A2D conversion
	void Set_A2D_Pin(unsigned char pinNum) {
		ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
		// Allow channel to stabilize
		static unsigned char i = 0;
		for ( i=0; i<15; i++ ) { asm("nop"); }
	}


static unsigned char GCD_Period = 10;

unsigned char numTasks = 2;

task part1, part4;
task *taskList[] = {&part1, &part4};

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

unsigned int joyc = 0;        // Keeps track of joystick
unsigned int joyc4 = 0;       // Keeps track of joystick (Up and Down for part4)
unsigned char C = 0x01;       // Pattern to display
unsigned char D = 0x01;		  // Rows enabled
enum part1 {wait, left, hold_left, right, hold_right} state = -1;
int part1_Tick() {
	switch(state) {//transitions
		case -1:
			state = wait;
		break;

		case wait:
			if(joyc > 155) {
				state = hold_left;
			}
			if(joyc < 50) {
				state = hold_right;
			}
		break;

		case left:
			state = wait;
		break;

		case hold_left:
			if(joyc < 155) {
				state = left;
			}
		break;

		case right:
			state = wait;
		break;

		case hold_right:
			if(joyc > 50) {
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
			//Make sure properly reset if stopped pressing joystick when bit disappears
			if(C < 0x01) {
				C = 0x01;
			}
		break;

		case hold_left:
			//if (C < 0x01) {
			//	C = 0x01;
			//}
			if(C < 0x80) {
				C = C << 1;
			}
		break;

		case right:
		break;

		case hold_right:
			//if (C < 0x01) {
			//	C = 0x80;
			//}
			if(C > 0x01) {
				C = C >> 1;
			}
		break;

		default:
			//Do nothing
		break;
	}
	PORTC = C;
	//PORTD = ~D;
	//return state;
}

enum part4 {wait4, down, hold_down, up, hold_up} state4 = -1;
int part4_Tick() {
	switch(state4) {//transitions
		case -1:
			state4 = wait4;
		break;

		case wait4:
			if(joyc4 > 90) {
				state4 = hold_down;
			}
			if(joyc4 < 60) {
				state4 = hold_up;
			}
		break;

		case down:
			state4 = wait4;
		break;

		case hold_down:
			if(joyc4 < 90) {
				state4 = down;
			}
		break;

		case up:
			state4 = wait4;
		break;

		case hold_up:
			if(joyc4 > 60) {
				state4 = up;
			}
		break;
		
		default:
			state4 = wait4;
		break;
	}

	switch(state4) {//actions
		case -1:
			//Do nothing
		break;

		case wait4:
			//Do nothing
		break;

		case down:
		break;

		case hold_down:
			if(D < 0x10) {
				D = D << 1;
			}
		break;

		case up:
		break;

		case hold_up:
			if (D > 0x01) {
				D = D >> 1;
			}
		break;

		default:
			//Do nothing
		break;
	}
	//PORTC = C;
	PORTD = ~D;
	//return state4;
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

 	part4.elapsedTime = part4.period;
 	part4.period = GCD_Period;
 	part4.state = -1;
 	part4.TickFct = &part4_Tick;
	
	A2D_init();	

    while(1) {
    	Set_A2D_Pin(0);
    	joyc = ADC;  // Get L/R joystick data
    	Set_A2D_Pin(1);
    	joyc4 = ADC; // Get U/D joystick data
		taskManage();
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
}