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

// External hardware include files
#include "keypad.h"
#include "lcd.h"
#include "bit.h"

// Button detection
#define BTN0 (~PINB & 0x01)
#define BTN1 (~PINB & 0x02)
#define BTN2 (~PINB & 0x04)
#define BTN3 (~PINB & 0x08)
#define BTN4 (~PINB & 0x10)
#define BTN5 (~PINB & 0x20)

unsigned char motorPhase[] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08}; // A, AB, B, BC, C, CD, D	
	
unsigned char key, press; // Keeps track of keypad input
unsigned char keyString[5]; //= { '\0', '\0', '\0', '\0' }; // Keeps track of aggregate keypad input
unsigned char motorStart = 0;

int i, j, k;

static int numDegrees;
static int numPhases; // Number of phases to rotate (D/5.625) * 64
			   // Degrees	Phases
			   // 90		1024
			   // 180		2048
			   // 360       4096

int degConv(int d) { // Converts the degree entered into the phase equivalent (D / 5.625) * 64
	return ((d / 5.625) * 64);
}

enum keypadState {INIT1, IDLE1, PRESS, UNPRESS} keypad_state;

void KEYPAD_Init() {
	keypad_state = INIT1;
}

void KEYPAD_Tick() {
	switch(keypad_state) { // Transitions
		case INIT1:
			k = 0;
			/*for(int g = 0; g < 4; ++ g) {
				keyString[g] = '\0';
			}*/
			keypad_state = IDLE1;
		break;

		case IDLE1:
			key = GetKeypadKey();

			if(key != '\0') { // Some keypad input
				keypad_state = PRESS;
			}
		break;

		case PRESS:
			press = GetKeypadKey();
			if(press != key) {
				keypad_state = UNPRESS;
			}
		break;

		case UNPRESS:
			keypad_state = IDLE1;
		break;

		default:
			keypad_state = INIT1;
		break;
	}

	switch(keypad_state) { // Actions
		case INIT1:
			// Do nothing
		break;

		case IDLE1:
			// Do nothing
		break;

		case PRESS:
			// Do nothing
		break;

		case UNPRESS:
			if(key == "*") { // Reset condition
				k = 0;
				keypad_state = IDLE1;
			}
			else if(key >= '0' && key <= '9' && k < 4) { // 0-9 and enough for the string
				keyString[k] = key;
				++ k;
			}
			else if(key == 'A' || key == 'B' && k != 0) {
				numDegrees = atoi(keyString);
				numPhases = degConv(numDegrees);
				if(key == 'A') {
					motorStart = 'A';
				}
				else {
					motorStart = 'B';
				}
				k = 0;
				keypad_state = IDLE1;
			}
			else {
				keypad_state = IDLE1;
			}
		break;

		default:
			// Do nothing
		break;
	}
}

void KeypadSecTask() {
    KEYPAD_Init();
    for(;;) { 	
		KEYPAD_Tick();
		vTaskDelay(2); 
   } 
}

enum motorState {INIT2, IDLE2, COUNTERCLOCKWISE, CLOCKWISE} motor_state;

void MOTOR_Init() {
	motor_state = INIT2;
}

void MOTOR_Tick() { // Transitions
	key = GetKeypadKey();
	switch(motor_state) { 
		case INIT2:
			motor_state = IDLE2;
		break;

		case IDLE2:
			if(motorStart == 'A') {
				motor_state = COUNTERCLOCKWISE;
			}
			if(motorStart == 'B') {
				motor_state = CLOCKWISE;
			}
		break;

		case COUNTERCLOCKWISE:
			if(j >= numPhases) {
				i = 0;
				j = 0;
				motorStart = 0;
				motor_state = IDLE2;
			}
		break;

		case CLOCKWISE:
			if(j >= numPhases) {
				i = 0;
			    j = 0;
			    motorStart = 0;
			    motor_state = IDLE2;
			}
		break;

		default:
			motor_state = INIT2;
		break;
	}

	switch(motor_state) { // Actions
		case INIT2:
			i = 0;
			j = 0;
		break;

		case IDLE2:
			// Do nothing
		
		break;

		case COUNTERCLOCKWISE:
			if(i < 0) {
				i = 6;
			}
			PORTA = motorPhase[i];
			-- i;
			++ j;
		break;

		case CLOCKWISE:
			if(i > 6) {
				i = 0;
			}
			PORTA = motorPhase[i];
			++ i;	
			++ j;
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
	xTaskCreate(KeypadSecTask, (signed portCHAR *)"KeypadSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(MotorSecTask, (signed portCHAR *)"MotorSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) { 
	DDRA = 0xFF; PORTA = 0x00; // Set PORTD as output
   	DDRB = 0x00; PORTB = 0xFF; // Set PORTB as input
   	DDRC = 0xFF; PORTC = 0x00; // Set PORTC as output
   	DDRD = 0xF0; PORTD = 0x0F; // Set PORTD for keypad

   	// Start Tasks  
   	StartSecPulse(1);
    // RunSchedular 
   	vTaskStartScheduler(); 
 
   	return 0; 
}