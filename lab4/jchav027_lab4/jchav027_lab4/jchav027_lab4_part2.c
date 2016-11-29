/* Coder: Juan Chavez
 * Date:  10.10.2015
 * Class: CS122A 
 * Lab 4 Part 2
 * Description: 
 */

/*#include <avr/io.h>
#include "scheduler.h"
#include "timer.h"

#define BTN1 (~PINA & 0x01)
#define BTN2 (~PINA & 0x02)

unsigned char op = 0xFF;

unsigned char GCD_Period = 10;

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

//Create a list of the SM's for the task list
unsigned char numTasks = 4;
//d for display
task part2_d1, part2_d2, part2_d3, part2_btn;
task *taskList[] = {&part2_d1, &part2_d2, &part2_d3, &part2_btn};

static unsigned int go = 1;

//Manages button press
enum part2_btnTicks {start, idle, off, hold, add, sub} part2_btnState = -1;
int part2_btnTickFunction(int state) {
	int tickState = state;

	switch(tickState) { //transitions
		case -1:
			tickState = start;
		break;

		case start:
			tickState = idle;
		break;

		case idle:
			if(BTN1 && BTN2) {
				tickState = off;
			}
			else if(BTN1 && go < 3) {
				tickState = add;
			}
			else if(BTN2 && go > 1) {
				tickState = sub;
			}
			else {
				tickState = idle;
			}
		break;

		case off:
			if (BTN1 && BTN2) {
				tickState = start;
			}
		break;
		
		case hold:
			if (BTN1 && BTN2) {
				tickState = hold;
			}		
			else {
				tickState = off;
			}

		case add: 
			tickState = idle;
		break;

		case sub:
			tickState = idle;
		break;
	}

	switch(tickState) { //actions
		case -1:
			//Do nothing
		break;

		case start:
			go = 1;
		break;

		case idle:
			//Do nothing
		break;

		case off:
			op = 0x00;
			go = 0;
			transmit_data(op);
		break;

		case hold:
			go = 0;
		break;
		
		case add:
			go ++;
		break;

		case sub:
			go --;
		break;
	}
	return tickState;
}

//First festive display's SM
int i1 = 0;
unsigned char d1_pattern[] = {0x55, 0xAA};
enum part2_d1Ticks {start1, active1, inactive1} part2_d1State = -1;
int part2_d1TickFunction(int state)
{
	int tickState = state;

	switch(tickState) { //transitions
		case -1:
			tickState = start1;
		break;
		
		case start1:
			tickState = inactive1;
		break;

		case active1:
			if(go != 1) {
				i1 = 0;
				tickState = inactive1;
			}
			else {
				if (i1 < 1) {
					i1 ++;
				}
				else {
					i1 = 0;
				}
			}
		break;

		case inactive1:
			if(go == 1) { //Display only works when go is 1 for the '1st' light display
				tickState = active1;
			}
		break;

		default:
			tickState = start1;	
		break;
	}

	switch(tickState) { //actions
		case -1:
			//Do nothing
		break;

		case start1:
			//Do nothing
		break;

		case active1:
			op = d1_pattern[i1];
			transmit_data(op);
		break;
		
		case inactive1:
			//Do nothing
		break;
		
		default:
			//Do nothing
		break;
	}	
	return tickState;
}

//Second festive display's SM
int i2 = 0;
unsigned char d2_pattern[] = { 0x39, 0xC6, 0x9C, 0x63 };
enum part2_d2Ticks {start2, active2, inactive2} part2_d2State = -1;
int part2_d2TickFunction(int state)
{
	int tickState = state;

	switch(tickState) //transitions
	{
		case -1:
			tickState = start2;
		break;
		
		case start2:
			tickState = inactive2;
		break;

		case active2:
			if(go != 2) {
				i2 = 0;
				tickState = inactive2;
			}
			else {
				if (i2 < 4) {
					i2 ++;
				}
				else {
					i2 = 0;
				}
			}
		break;

		case inactive2:
			if(go == 2) { //Display only works when go is 1 for the '1st' light display
				tickState = active2;
			}
		break;

		default:
			tickState = start2;
		break;
	}

	switch(tickState) //actions
	{
		case -1:
			//Do nothing
		break;

		case start2:
			//Do nothing
		break;

		case active2:
			op = d2_pattern[i2];
			transmit_data(op);
		break;
		
		case inactive2:
			//Do nothing
		break;
		
		default:
			//Do nothing
		break;
	}
	return tickState;
}

//Third festive display's SM
int i3 = 0;
unsigned char d3_pattern[] = { 0x81, 0x42, 0x24, 0x18, 0x24, 0x42 };
enum part2_d3Ticks {start3, active3, inactive3} part2_d3State = -1;
int part2_d3TickFunction(int state)
{
	int tickState = state;

	switch(tickState) //transitions
	{
		case -1:
			tickState = start3;
		break;
		
		case start3:
			tickState = inactive3;
		break;

		case active3:
			if(go != 3) {
				i3 = 0;
				tickState = inactive3;
			}
			else {
				if (i3 < 6) {
					i3 ++;
				}
				else {
					i3 = 0;
				}
			}
		break;

		case inactive3:
			if(go == 3) { //Display only works when go is 1 for the '1st' light display
				tickState = active3;
			}
		break;

		default:
			tickState = start3;
		break;
	}

	switch(tickState) //actions
	{
		case -1:
			//Do nothing
		break;

		case start3:
			//Do nothing
		break;

		case active3:
			op = d3_pattern[i3];
			transmit_data(op);
		break;
		
		case inactive3:
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
		if (taskList[i]->elapsedTime == taskList[i]->period) {
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
	DDRC = 0xFF; PORTC = 0x00; //Set port C's 8 pins as outputs
	
	TimerSet(GCD_Period);
	TimerOn();
 
 	//Set up
 	//First light display
 	part2_d1.elapsedTime = part2_d1.period;
 	part2_d1.period = GCD_Period;
 	part2_d1.state = -1;
 	part2_d1.TickFct = &part2_d1TickFunction;
	
	//Second light display
 	part2_d2.elapsedTime = part2_d2.period;
 	part2_d2.period = GCD_Period;
 	part2_d2.state = -1;
 	part2_d2.TickFct = &part2_d2TickFunction;

	//Third light display
 	part2_d3.elapsedTime = part2_d3.period;
 	part2_d3.period = GCD_Period;
 	part2_d3.state = -1;
 	part2_d3.TickFct = &part2_d3TickFunction;

	//Button management
 	part2_btn.elapsedTime = part2_btn.period;
 	part2_btn.period = GCD_Period;
 	part2_btn.state = -1;
 	part2_btn.TickFct = &part2_btnTickFunction;

    while(1) {
		taskManage();
		
		while(!TimerFlag);
			TimerFlag = 0;
    }
}*/