/* Name:  Juan Chavez
 * Email: jchav027@ucr.edu
 * Date:  10.21.2015
 * Class: CS122A
 * Lab:   7 Part 3
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
#include "FreeRTOS.h"
#include "usart_ATmega1284.h"

// Button detection
#define BTN1 (~PINB & 0x01)

enum part2States {Init1, Get, Toggle} part2State;
static unsigned int period = 100;
unsigned char LED1 = 0;
unsigned char aut = 0;
	
void part2_init() {
	part2State = Init1;
}

void part2_tick() {	
	//Transitions
	switch(part2State) {	
		case Init1:
			part2State = Get;
		break;
		
		case Get:
			PORTC = 0x00;
			/*if(BTN1) {
				period = 100;
				part2State = Toggle;
			}*/
			
			//else {
				if(!USART_HasReceived(0)) {
					++aut;
				}

				if(aut == 3) {
					aut = 0;
					period = 100;
					part2State = Toggle;
				}
			//}
		break;

		case Toggle:
			PORTC = 0x01;
			/*if(!BTN1) {
				period = 60;
				part2State = Get;
			}*/
			
			//else {
				if(USART_HasReceived(0)) {
					period = 60;
					part2State = Get;
				}
			//}
		break;
		
		default:
		
		break;
	}
	
	//Actions
	switch(part2State) {
		case Init1:
			// Do nothing
		break;
		
		case Get: // Behaves like a follower
			if(USART_HasReceived(0)) {
				LED1 = USART_Receive(0);
				USART_Flush(0);
				PORTA = LED1;
			}
		break;
		
		case Toggle: // Behaves like the leader
			//if(USART_IsSendReady(1)) {
				//PORTA = LED1;
				USART_Send(LED1, 1);
			//}
			PORTA = LED1;
			LED1 ^= 0x01;
		
			//if(USART_HasTransmitted(0)) {
				//LED1 ^= 0x01;
				//PORTA = LED1;
			//}
		break;

		default:
		
		break;
	}
}

void LEDSecTask() {
	part2_init();
	for(;;) {
		part2_tick();
		vTaskDelay(period);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LEDSecTask, (signed portCHAR *)"LEDSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

int main(void) {
	DDRA = 0xFF; PORTA = 0x00; // Set PORTA to output
	DDRC = 0xFF; PORTC = 0x00; // Set PORTC to output
	DDRD = 0x00; PORTD = 0xFF; // Set PORTD to input
	DDRB = 0x00; PORTB = 0xFF; // Set PORTB to input
	//DDRD = 0x0A; PORTD = 0xF5; // Set PORTD as input
	
	initUSART(0); // Used to receive data
	initUSART(1); // Used to send data
	
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}