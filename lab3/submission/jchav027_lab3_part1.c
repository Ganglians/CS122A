/* Coder:     Juan Chavez
 * Class:     CS122A Lab3 Part1
 * Section:   23
 * Date:      10.6.2015
 * Objective: Learn to use FreeRTOS.
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
 
//FreeRTOS include files 

#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 

#define BTN !(PINA & 0x01)

 unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum LEDState_1 {INIT_1, ON_1, OFF_1} led_state_1;

void LEDS_Init_1(){
	led_state_1 = INIT_1;
}

void LEDS_Tick_1(){
	//Actions
	switch(led_state_1){
		case INIT_1:
			PORTD = 0x15;
		break;

		case ON_1:
			PORTD = 0x15;
		break;

		case OFF_1:
			PORTD = 0x00;
		break;

		default:
			PORTD = 0x00;
		break;
	}
	//Transitions
	switch(led_state_1){
		case INIT_1:
			led_state_1 = ON_1;
		break;

		case ON_1:
			led_state_1 = OFF_1;
		break;

		case OFF_1:
			led_state_1 = ON_1;
		break;

		default:
			led_state_1 = INIT_1;
		break;
	}
}

void LedSecTask_1()
{
	LEDS_Init_1();
   for(;;) 
   { 	
	LEDS_Tick_1();
	vTaskDelay(100); 
   } 
}

void StartSecPulse_1(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask_1, (signed portCHAR *)"LedSecTask_1", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRD = 0xFF;

   //Start tasks 
   StartSecPulse_1(1);

   vTaskStartScheduler(); 
 
   return 0; 
}