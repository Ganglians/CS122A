/* Name:  Juan Chavez
 * Email: jchav027@ucr.edu
 * Date:  10.21.2015
 * Class: CS122A
 * Lab:   7 Part 1 (Leader)
 * Description:
 */

/*#include <stdint.h>
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

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "FreeRTOS.h"
#include "usart_ATmega1284.h"

enum part1States {Init1, Toggle} part1State;
unsigned char LED1 = 0;

void part1_init() {
	part1State = Init1;
}

void part1_tick() {
	//Actions
	switch(part1State) {
		case Init1:
			LED1 = 0x00;
		break;
		
		case Toggle:
			if(USART_IsSendReady(0)) {
				//PORTA = LED1;
				USART_Send(LED1, 0);
				PORTA = LED1;
				LED1 ^= 0x01;
			}
			//if(USART_HasTransmitted(0)) {
				//LED1 ^= 0x01;
				//PORTA = LED1;
			//}
		break;
		
		default:
		
		break;
	}
	
	//Transitions
	switch(part1State) {
		case Init1:
			part1State = Toggle;
		break;
		
		case Toggle:
			// Do nothing (i.e. stay in this state)		
		break;
		
		default:
		
		break;
	}
}

void LeadSecTask() {
	part1_init();
	for(;;) {
		part1_tick();
		vTaskDelay(50);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LeadSecTask, (signed portCHAR *)"LeadSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

int main(void) {
	DDRA = 0xFF; PORTA = 0x00; // Set PORTA to output
	DDRD = 0xFF; PORTD = 0x00; // Set PORTD to output
	//DDRD = 0x0A; PORTD = 0xF5; // Set PORTD as input
	
	initUSART(0);
	
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}*/