/* Coder:     Juan Chavez
 * Class:     CS122A Lab3 Part3
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

volatile unsigned char tmpD = 0x00;

enum LEDState_3 {INIT_3,FWD,REV} led_state_3;

void LEDS_Init_3(){
	led_state_3 = INIT_3;
}

void LEDS_Tick_3(){
	//Actions
	switch(led_state_3){
		case INIT_3:
			PORTD = 1;
		break;
		case FWD:
			PORTD = PORTD << 1;
		break;
		case REV:
			PORTD = PORTD >> 1;
		break;
		default:
			PORTD = 0;
		break;
	}
	//Transitions
	switch(led_state_3){
		case INIT_3:
			led_state_3 = FWD;
		break;
		case FWD:
			if(PORTD == 0x80) {
				led_state_3 = REV;
			}
		break;
		case REV:
			if(PORTD == 0x01) {
				led_state_3 = FWD;
			}
		break;
		default:
			led_state_3 = INIT_3;
		break;
	}
}

void LedSecTask_3()
{
	LEDS_Init_3();
   for(;;) 
   { 	
	LEDS_Tick_3();
	vTaskDelay(100); 
   } 
}

void StartSecPulse_3(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask_3, (signed portCHAR *)"LedSecTask_3", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRD = 0xFF;

   //Start tasks 
   StartSecPulse_3(1);

   vTaskStartScheduler(); 
 
   return 0; 
}