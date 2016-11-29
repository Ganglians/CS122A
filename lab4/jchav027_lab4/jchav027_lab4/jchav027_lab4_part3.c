/* Coder: Juan Chavez
 * Date:  10.10.2015
 * Class: CS122A 
 * Lab 4 Part 3
 * Description: 
 */

/*#include <avr/io.h>
#include "scheduler.h"
#include "timer.h"

#define BTN1 (~PINA & 0x01)
#define BTN2 (~PINA & 0x02)
#define BTN3 (~PINA & 0x04)
#define BTN4 (~PINA & 0x08)

unsigned char op = 0xFF;
unsigned char op2 = 0xFF;

unsigned char GCD_Period = 10;

//TODO: Modify the �transmit_data� function so that different RCLK and SRCLR lines are
//      used depending on which shift register number is passed as a parameter.

void transmit_data(unsigned char data, unsigned int reg) {
	int i;
	for (i = 7; i >= 0 ; --i) {
		//TODO: Change to send signal to register 1 or register 2

		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		//SRCLR:
		if(reg == 1) {
			PORTC = 0x08; //First register 
		}
		else {
		    PORTC = 0x20; //Second register
		}

		//***Shared
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x04;
		//***End Shared
	}
	// set RCLK = 1. Rising edge copies data from the �Shift� register to the �Storage� register
	//RCLK:
	if(reg == 1) {
		PORTC |= 0x02; //First register
	}
	else {
		PORTC = 0x10; //Second register
	}

	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
}

//Create a list of the SM's for the task list
unsigned char numTasks = 8;
//d for display
task part2_d1, part2_d2, part2_d3, part2_btn, part2_d12, part2_d22, part2_d32, part2_btn2;
task *taskList[] = {&part2_d1, &part2_d2, &part2_d3, &part2_btn, &part2_d12, &part2_d22, &part2_d32, &part2_btn2};

static unsigned int go = 1;
static unsigned int go2 = 1;

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
			transmit_data(op, 1);
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
			transmit_data(op, 1);
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
			transmit_data(op, 1);
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
			transmit_data(op, 1);
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

//PART2

//Manages button press
enum part2_btnTicks2 {start_2, idle2, off2, hold2, add2, sub2} part2_btnState2 = -1;
int part2_btnTickFunction2(int state) {
	int tickState = state;

	switch(tickState) { //transitions
		case -1:
		tickState = start_2;
		break;

		case start_2:
		tickState = idle2;
		break;

		case idle2:
		if(BTN3 && BTN4) {
			tickState = off2;
		}
		else if(BTN3 && go2 < 3) {
			tickState = add2;
		}
		else if(BTN4 && go2 > 1) {
			tickState = sub2;
		}
		else {
			tickState = idle2;
		}
		break;

		case off2:
		if (BTN3 && BTN4) {
			tickState = start_2;
		}
		break;
		
		case hold2:
		if (BTN3 && BTN4) {
			tickState = hold2;
		}
		else {
			tickState = off2;
		}

		case add2:
		tickState = idle2;
		break;

		case sub2:
		tickState = idle2;
		break;
	}

	switch(tickState) { //actions
		case -1:
		//Do nothing
		break;

		case start_2:
		go2 = 1;
		break;

		case idle2:
		//Do nothing
		break;

		case off2:
		op2 = 0x00;
		go2 = 0;
		transmit_data(op2, 2);
		break;

		case hold2:
		go2 = 0;
		break;
		
		case add2:
		go2 ++;
		break;

		case sub2:
		go2 --;
		break;
	}
	return tickState;
}


//First festive display's SM
int i1_2 = 0;
unsigned char d1_pattern2[] = {0x55, 0xAA};
enum part2_d1Ticks2 {start21, active21, inactive21} part2_d1State2 = -1;
int part2_d1TickFunction2(int state)
{
	int tickState = state;

	switch(tickState) { //transitions
		case -1:
			tickState = start21;
		break;
		
		case start21:
			tickState = inactive21;
		break;

		case active21:
			if(go2 != 1) {
				i1_2 = 0;
				tickState = inactive21;
			}
			else {
				if (i1_2 < 1) {
					i1_2 ++;
				}
				else {
					i1_2 = 0;
				}
			}
		break;

		case inactive21:
			if(go2 == 1) { //Display only works when go2 is 1 for the '1st' light display
				tickState = active21;
			}
		break;

		default:
			tickState = start21;	
		break;
	}

	switch(tickState) { //actions
		case -1:
			//Do nothing
		break;

		case start21:
			//Do nothing
		break;

		case active21:
			op2 = d1_pattern2[i1_2];
			transmit_data(op2, 2);
		break;
		
		case inactive21:
			//Do nothing
		break;
		
		default:
			//Do nothing
		break;
	}	
	return tickState;
}

//Second festive display's SM
int i2_2 = 0;
unsigned char d2_pattern2[] = { 0x39, 0xC6, 0x9C, 0x63 };
enum part2_d2Ticks2 {start22, active22, inactive22} part2_d2State2 = -1;
int part2_d2TickFunction2(int state)
{
	int tickState = state;

	switch(tickState) //transitions
	{
		case -1:
			tickState = start22;
		break;
		
		case start22:
			tickState = inactive22;
		break;

		case active22:
			if(go2 != 2) {
				i2_2 = 0;
				tickState = inactive22;
			}
			else {
				if (i2_2 < 4) {
					i2_2 ++;
				}
				else {
					i2_2 = 0;
				}
			}
		break;

		case inactive22:
			if(go2 == 2) { //Display only works when go2 is 1 for the '1st' light display
				tickState = active22;
			}
		break;

		default:
			tickState = start22;
		break;
	}

	switch(tickState) //actions
	{
		case -1:
			//Do nothing
		break;

		case start22:
			//Do nothing
		break;

		case active22:
			op2 = d2_pattern2[i2_2];
			transmit_data(op, 2);
		break;
		
		case inactive22:
			//Do nothing
		break;
		
		default:
			//Do nothing
		break;
	}
	return tickState;
}

//Third festive display's SM
int i3_2 = 0;
unsigned char d3_pattern2[] = { 0x81, 0x42, 0x24, 0x18, 0x24, 0x42 };
enum part2_d3Ticks2 {start23, active23, inactive23} part2_d3State2 = -1;
int part2_d3TickFunction2(int state)
{
	int tickState = state;

	switch(tickState) //transitions
	{
		case -1:
			tickState = start23;
		break;
		
		case start23:
			tickState = inactive23;
		break;

		case active23:
			if(go2 != 3) {
				i3_2 = 0;
				tickState = inactive23;
			}
			else {
				if (i3_2 < 6) {
					i3_2 ++;
				}
				else {
					i3_2 = 0;
				}
			}
		break;

		case inactive23:
			if(go2 == 3) { //Display only works when go2 is 1 for the '1st' light display
				tickState = active23;
			}
		break;

		default:
			tickState = start23;
		break;
	}

	switch(tickState) //actions
	{
		case -1:
			//Do nothing
		break;

		case start23:
			//Do nothing
		break;

		case active23:
			op2 = d3_pattern2[i3_2];
			transmit_data(op2, 2);
		break;
		
		case inactive23:
			//Do nothing
		break;
		
		default:
			//Do nothing
		break;
	}
	return tickState;
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
	 
	//Part 2:
	
 	//First light display
 	part2_d12.elapsedTime = part2_d12.period;
 	part2_d12.period = GCD_Period;
 	part2_d12.state = -1;
 	part2_d12.TickFct = &part2_d1TickFunction2;
 	
 	//Second light display
 	part2_d22.elapsedTime = part2_d22.period;
 	part2_d22.period = GCD_Period;
 	part2_d22.state = -1;
 	part2_d22.TickFct = &part2_d2TickFunction2;

 	//Third light display
 	part2_d32.elapsedTime = part2_d32.period;
 	part2_d32.period = GCD_Period;
 	part2_d32.state = -1;
 	part2_d32.TickFct = &part2_d3TickFunction2;

 	//Button management
 	part2_btn2.elapsedTime = part2_btn2.period;
 	part2_btn2.period = GCD_Period;
 	part2_btn2.state = -1;
 	part2_btn2.TickFct = &part2_btnTickFunction2;
    while(1) {
		taskManage();
		
		while(!TimerFlag);
			TimerFlag = 0;
    }
}*/