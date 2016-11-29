/* Name:  Juan Chavez
 * Email: jchav027@ucr.edu
 * Date:  10.29.2015
 * Class: CS122A
 * Lab:   9
 * Description:
 */

#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <avr/pgmspace.h> 
 
// FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 

// Button detection
#define BTN1 (~PINB & 0x01)
#define BTN2 (~PINB & 0x02)

unsigned char motorPhase[] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08}; // A, AB, B, BC, C, CD, D
	
int i;
enum motorState {INIT, IDLE, COUNTERCLOCKWISE, CLOCKWISE} motor_state;

void MOTOR_Init() {
	motor_state = INIT;
}

void MOTOR_Tick() { // Transitions
	switch(motor_state) { 
		case INIT:
			motor_state = IDLE;
		break;

		case IDLE:
			if(BTN1) {
				i = 6;
				motor_state = COUNTERCLOCKWISE;
			}
			else if(BTN2) {
				i = 0;
				motor_state = CLOCKWISE;
			}
			else {
				// Do nothing
			}
		break;

		case COUNTERCLOCKWISE:
			if(BTN1) {
				// Do nothing
			}
			else if(BTN2) {
				i = 0;
				motor_state = CLOCKWISE;
			}
			else {
				motor_state = IDLE;
			}
		break;

		case CLOCKWISE:
			if(BTN1) {
				i = 6;
				motor_state = COUNTERCLOCKWISE;
			}
			else if(BTN2) {
				// Do nothing
			}
			else {
				motor_state = IDLE;
			}
		break;

		default:
			motor_state = INIT;
		break;
	}
	switch(motor_state) { // Actions
		case INIT:
			i = 0;
		break;

		case IDLE:
			// Do nothing
		break;

		case COUNTERCLOCKWISE:
			if(i < 0) {
				i = 6;
			}
			PORTA = motorPhase[i];
			-- i;
		break;

		case CLOCKWISE:
			if(i > 6) {
				i = 0;
			}
			PORTA = motorPhase[i];
			++ i;
		break;
		
		default:
			// Do nothing
		break;
	}
}

void MotorSecTask() {
	MOTOR_Init();
   for(;;) { 	
	MOTOR_Tick();
	vTaskDelay(1); 
   } 
}

void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(MotorSecTask, (signed portCHAR *)"MotorSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) { 
   DDRB = 0x00; PORTB = 0xFF; // Set PORTB as input
   DDRC = 0x00; PORTC = 0xFF;
   DDRA = 0xFF; PORTA = 0x00; // Set PORTA as output
   DDRD = 0xFF; PORTD = 0x00;

   // Start Tasks  
   StartSecPulse(1);
    // RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}