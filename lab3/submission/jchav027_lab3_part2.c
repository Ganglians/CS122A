/* Coder:     Juan Chavez
 * Class:     CS122A Lab3 Part2
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

//1st LED
enum D0_S{INIT_20, on0, off0} led_state_20;

void LEDS_Init_20(){
	led_state_20 = INIT_20;
}
	
void toggleD0() {
	switch(led_state_20) { //transitions
		case INIT_20:
			led_state_20 = on0;
			break;
			
		case on0:
			led_state_20 = off0;
			break;
			
		case off0:
			led_state_20 = on0;
			break;
			
		default:
			led_state_20 = INIT_20;
	}
	
	switch(led_state_20) { //actions
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
	//return state;
} 

void LedSecTask_20()
{
	LEDS_Init_20();
   for(;;) 
   { 	
	toggleD0();
	vTaskDelay(50); 
   } 
}

void StartSecPulse_20(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask_20, (signed portCHAR *)"LedSecTask_20", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

//2nd LED
enum D2_S{INIT_22, on2, off2} led_state_22;

void LEDS_Init_22(){
	led_state_22 = INIT_22;
}

void toggleD2(){
	switch(led_state_22) {
		case INIT_22:
			led_state_22 = on2;
			break;
		
		case on2:
			led_state_22 = off2;
			break;
		
		case off2:
			led_state_22 = on2;
			break;
		
		default:
			led_state_22 = INIT_22;
	}
	
	switch(led_state_22) {
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
	//return state;
}

void LedSecTask_22()
{
	LEDS_Init_22();
   for(;;) 
   { 	
	toggleD2();
	vTaskDelay(100); 
   } 
}

void StartSecPulse_22(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask_22, (signed portCHAR *)"LedSecTask_22", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

//4th LED
enum D4_S{INIT_24, on4, off4} led_state_24;
	
void LEDS_Init_24(){
	led_state_24 = INIT_24;
}

void toggleD4(){
	switch(led_state_24) {
		case INIT_24:
			led_state_24 = on4;
			break;
		
		case on4:
			led_state_24 = off4;
			break;
		
		case off4:
			led_state_24 = on4;
			break;
		
		default:
			led_state_24 = INIT_24;
	}
	
	switch(led_state_24) {
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
	//return state;
}

void LedSecTask_24()
{
	LEDS_Init_24();
   for(;;) 
   { 	
	toggleD4();
	vTaskDelay(250); 
   } 
}

void StartSecPulse_24(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask_24, (signed portCHAR *)"LedSecTask_24", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}


int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRD = 0xFF;

   //Start tasks 
   StartSecPulse_20(1);
   StartSecPulse_22(1);
   StartSecPulse_24(1);

   vTaskStartScheduler(); 
 
   return 0; 
}