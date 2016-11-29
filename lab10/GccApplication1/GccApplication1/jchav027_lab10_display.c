/* Name:  Juan Chavez
 * Email: jchav027@ucr.edu
 * Date:  11.16.2015
 * Class: CS122A
 * Lab:   10
 * Description: Custom lab
 */

////////////////////////////////////////////////////////////////////////////////
// Libraries
#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>

#include "scheduler.h"
#include "usart_ATmega1284.h"

// External hardware header files
#include "lcd.h"

///////////////////////////////////////////////////////////////////////////////
// Global variables

#define BUFFSIZE 5

 char buff[BUFFSIZE]; // Prints out score

// Dictates timer's period
static unsigned char GCD_Period = 100;

// Keeps track of joystick position
unsigned int joyc = 0;

// Keeps track of total score(i.e. coins available)
static unsigned int coins = 20; 

static unsigned int lightBase;

uint8_t EEMEM TEST;

///////////////////////////////////////////////////////////////////////////////
// Helper functions
void printScore() {
	int cursorStart = 9;

	for(int i = 0; buff[i] && i < BUFFSIZE; ++ i) {
		LCD_Cursor(cursorStart);
		LCD_WriteData(buff[i]);
		++ cursorStart;
	}
}
///////////////////////////////////////////////////////////////////////////////
// Analog to digital functions
void adc_init() {
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch) {
	// select the corresponding channel 0~7
	// ANDing with ’7? will always keep the value
	// of ‘ch’ between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write ’1? to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0? again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}

///////////////////////////////////////////////////////////////////////////////
// Coin detection tick function
enum coinState {INIT1, IDLE1, DETECT, ADD, CHECK, UNLOCK} coin_state ;

static unsigned int lightCurr = 0;

void COIN_Tick() {
	switch(coin_state) { // Transitions
		case INIT1:
			LCD_DisplayString(1, "Score:");

			itoa(coins, buff, 10);
			printScore();

			coin_state = IDLE1;
		break;
		
		case IDLE1:
			if(USART_HasReceived(0)) {
				unsigned char tmp = USART_Receive(0);
				USART_Flush(0);
				coins += tmp;
				coin_state = CHECK;
			}
			// if(lightCurr >= lightBase) { 
			// 	coin_state = IDLE1;
			// }
			// else {
			// 	coin_state = DETECT;
			// }
		break;
		
		case DETECT:
			if(lightCurr < lightBase) { 
				//tempB = 0x81;
				//PORTB = tempB;
				coin_state = DETECT;
			}
			else {
				//tempB = 0x80;
				//PORTB = tempB;
				coin_state = ADD; 
			}
		break;
		
		case ADD:
			coin_state = CHECK;
		break;

		case CHECK:
			if(coins >= 100) {
				coin_state = UNLOCK;
			}
			else {
				coin_state = INIT1;
			}
		break;

		case UNLOCK:
			coin_state = INIT1;
		break;
		
		default:
			coin_state = INIT1;
		break;
	}
	
	switch(coin_state) { // Actions
		case INIT1:
			//normLight = adc_read(2);
			//coins = 0;
		break;
		
		case IDLE1:
			//lightCurr = adc_read(1);
			//a0 = adc_read(2);
		break;
		
		case DETECT:
			//lightCurr = adc_read(1);
			//a0 = adc_read(2);
		break;
		
		case ADD:
			++ coins;
		break;
		
		case CHECK:
			// Do nothing
		break;

		case UNLOCK:
			PORTB = 0x00;
		break;

		default:
			// Do nothing
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Joystick pulldown tick function

// Joystick variables
enum joyState {INIT2, IDLE2, PULLDOWN, DONE} joy_state;

// Joystick's tick function
int JOY_Tick() {
	// Sample joystick output
	joyc = adc_read(0);
	//joyc = ADC;
	switch(joy_state) { // Transitions
		case INIT2:
			joy_state = IDLE2;
		break;

		case IDLE2:	
			// if(USART_HasReceived(0)) {
			// 	//LCD_DisplayString(1, "Got");
			// 	unsigned char tmp = USART_Receive(0);
			// 	USART_Flush(0);
			// 	coins += tmp;
			// }
			if(joyc < 1019) {
				// Do nothing
			}

			else {
				joy_state = PULLDOWN;
			}
		break;

		case PULLDOWN:
			if(joyc < 1019) {
				joy_state = DONE;
			}

			else {
				// Stay in same state
			}
		break;

		case DONE:
			//LCD_DisplayString(7, "IDLE");
			//SPI_MasterTransmit(0);
			joy_state = IDLE2;
		break;

		default:
			joy_state = INIT2;
		break;
	}

	switch(joy_state) { // Actions 
		case INIT2:
			// Do nothing
		break;

		case IDLE2:
			// Do nothing
		break;

		case PULLDOWN:
			// Do nothing
		break;

		case DONE:
			// Send USART signal
			USART_Send(1, 1);
		break;

		default:
			// Do nothing
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Main function
int main(void) {
	//DDRA = 0x00; PORTA = 0xFF; // A reads joystick inputs
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00; // B is output to electromagnet 
	DDRC = 0xFF; PORTC = 0x00; // C is output to LCD 
	DDRD = 0xFF; PORTD = 0x00; // D outputs RS and E signals to LCD

	PORTB = 0x01; // Magnet initially on
	//PORTB = 0xFF;

	// Initialize UART 
	initUSART(0); // Used to receive data
	initUSART(1); // Used to send data

	// Initialize LCD
	LCD_init();
	
	// Initialize analog to digital converter
	//A2D_init();
	adc_init();

	// Base value for light intensity
	lightBase = adc_read(1) + 100;

	// Scheduler task setup
	tasksNum = 2;    		   // Declare number of tasks
	task myTasks[tasksNum];    // Initialize the task array
	tasks = myTasks; 		   // Set the task array

	//eeprom_write_byte(&TEST, 8);

	uint8_t SRAM_Test;
	SRAM_Test = eeprom_read_byte(&TEST);

	//LCD_Cursor(1);
	//LCD_WriteData(('0' + SRAM_Test));

	// Task initialization
	unsigned char _i = 0;
	tasks[_i].state = -1;
	tasks[_i].period = 4;
	tasks[_i].elapsedTime = tasks[_i].period;
	tasks[_i].TickFct = &COIN_Tick;

	++ _i;

	tasks[_i].state = -1;
	tasks[_i].period = 4;
	tasks[_i].elapsedTime = tasks[_i].period;
	tasks[_i].TickFct = &JOY_Tick;

	++ _i;

	// Initialize timer
	TimerSet(GCD_Period);
	TimerOn();

    while (1) {

    }
}