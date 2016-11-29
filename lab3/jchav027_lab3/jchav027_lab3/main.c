/* Coder:     Juan Chavez
 * Class:     CS122A
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
/*enum LEDState {INIT,L0,L1,L2,L3,L4,L5,L6,L7} led_state;

void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		PORTD = 0;
		break;
		case L0:
		PORTD = 1;
		break;
		case L1:
		PORTD = 2;
		break;
		case L2:
		PORTD = 4;
		break;
		case L3:
		PORTD = 8;
		break;
		case L4:
		PORTD = 16;
		break;
		case L5:
		PORTD = 32;
		break;
		case L6:
		PORTD = 64;
		break;
		case L7:
		PORTD = 128;
		break;
		default:
		PORTD = 0;
		break;
	}
	//Transitions
	switch(led_state){
		case INIT:
			led_state = L0;
		break;
		case L0:
			led_state = L1;
		break;
		case L1:
			led_state = L2;
		break;
		case L2:
			led_state = L3;
		break;
		case L3:
			led_state = L4;
		break;
		case L4:
			led_state = L5;
		break;
		case L5:
			led_state = L6;
		break;
		case L6:
			led_state = L7;
		break;
		case L7:
			led_state = L0;
		break;
		default:
			led_state = INIT;
		break;
	}
}

void LedSecTask()
{
	LEDS_Init();
   for(;;) 
   { 	
	LEDS_Tick();
	vTaskDelay(100); 
   } 
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}*/	

//Part 1
/*enum LEDState_1 {INIT_1, ON_1, OFF_1} led_state_1;

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
}*/

//Part 2
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

//Part 3
//volatile unsigned char tmpD = 0x00;

/*enum LEDState_3 {INIT_3,FWD,REV} led_state_3;

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
}*/

//Part 4
//volatile unsigned char tmpD = 0x00;

/*enum LEDState_4 {INIT_4,FWD,REV} led_state_4;

void LEDS_Init_4(){
	led_state_4 = INIT_4;
}

void LEDS_Tick_4(){
	//Actions
	switch(led_state_4){
		case INIT_4:
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
	switch(led_state_4){
		case INIT_4:
			led_state_4 = FWD;
		break;
		case FWD:
			if((PORTD == 0x80)|| BTN) {
				led_state_4 = REV;
			}
		break;
		case REV:
			if((PORTD == 0x01) || BTN) {
				led_state_4 = FWD;
			}
		break;
		default:
			led_state_4 = INIT_4;
		break;
	}
}

void LedSecTask_4()
{
	LEDS_Init_4();
   for(;;) 
   { 	
	LEDS_Tick_4();
	vTaskDelay(50); 
   } 
}

void StartSecPulse_4(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask_4, (signed portCHAR *)"LedSecTask_4", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}*/

int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRD = 0xFF;

   //Start Tasks  
   //StartSecPulse(1);

   //Start tasks 
   //StartSecPulse_1(1);
   
   //Start tasks for part 1
   //StartSecPulse_2(1);

   //Start tasks for part 2
   StartSecPulse_20(1);
   StartSecPulse_22(1);
   StartSecPulse_24(1); 

   //Start tasks for part 3
	//StartSecPulse_3(1);

   //Start tasks for part 4
   //StartSecPulse_4(1);
   //Run Scheduler 
   vTaskStartScheduler(); 
 
   return 0; 
}