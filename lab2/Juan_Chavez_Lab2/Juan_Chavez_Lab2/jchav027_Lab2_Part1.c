/* Coder: Juan Chavez
 * CS122A Lab2 part 1
 */


#include <avr/io.h>
#include <scheduler.h>

volatile unsigned char tmpD = 0x00;

enum D0_S{on0, off0};
	
int toggleD0(int state) {	
	switch(state) {
		case -1:
			state = on0;
			break;
			
		case on0:
			state = off0;
			break;
			
		case off0:
			state = on0;
			break;
			
		default:
			state = -1;
	}
	
	switch(state) {
		case on0:
			tmpD |= 0x01;
			break;
		
		case off0:
			tmpD &= 0xFE;
			break;
			
		default:
			break;
	}
	
	PORTD = tmpD;
	return state;
}
	
enum D2_S{on2, off2};

int toggleD2(int state) {
	switch(state) {
		case -1:
			state = on2;
			break;
		
		case on2:
			state = off2;
			break;
		
		case off2:
			state = on2;
			break;
		
		default:
			state = -1;
	}
	
	switch(state) {
		case on2:
			tmpD |= 0x04;
			break;
		
		case off2:
			tmpD &= 0xFB;
			break;
		
		default:
			break;
	}
	
	PORTD = tmpD;
	return state;
}
	
enum D4_S{on4, off4};
	
int toggleD4(int state) {
	switch(state) {
		case -1:
			state = on4;
			break;
		
		case on4:
			state = off4;
			break;
		
		case off4:
			state = on4;
			break;
		
		default:
			state = -1;
	}
	
	switch(state) {
		case on4:
			tmpD |= 0x10;
			break;
		
		case off4:
			tmpD &= 0xEF;
			break;
		
		default:
			break;
	}
	
	PORTD = tmpD;
	return state;
}

int main(void) {
	DDRD = 0xFF; PORTD = 0x00;
	
	int taskNum = 3;
	
	task function[taskNum];
	tasks = function;
	unsigned char i = 0;
	
	tasks[i].state = -1;
	tasks[i].period = 500;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &toggleD0;
	++i;
	
	tasks[i].state = -1;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &toggleD2;
	++i;
	
	tasks[i].state = -1;
	tasks[i].period = 2500;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &toggleD4;
	
	TimerSet(500);
	TimerOn();
	
    while(1){
    }
}