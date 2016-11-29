/* Name:  Juan Chavez
 * Email: jchav027@ucr.edu
 * Date:  10.29.2015
 * Class: CS122A
 * Lab:   8 (Master)
 * Description:
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "bit.h"
#include "lcd.h"
#include "scheduler.h"

unsigned char receivedData;
unsigned char array[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
unsigned char ascii[] = {49, 50, 51, 52, 53, 54, 55, 56, 57};

void SPI_MasterInit(void) {
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	DDRB = (1 << 4) | (1 << 5) | (1 << 7);
	// Set SPCR register to enable SPI, enable master, and use SCK frequency
	// of fosc/16  (pg. 168)
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	// Make sure global interrupts are enabled on SREG register (pg. 9)
	sei();
}

void SPI_MasterTransmit(unsigned char cData) {
	// data in SPDR will be transmitted
	SPDR = cData;
	// set SS low
	PORTB &= (0 << 4);
	while(!(SPSR & (1<<SPIF))) { // wait for transmission to complete
		;
	}
	// set SS high
	PORTB |= (1 << 4);
}

// Servant code
void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	DDRB = (1 << 6);
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR = (1 << SPE) | (1 << 7);
	// make sure global interrupts are enabled on SREG register (pg. 9)
	sei();
}

ISR(SPI_STC_vect) { // this is enabled in with the SPCR register’s “SPI
	// SPDR contains the received data, e.g. unsigned char 
	receivedData = SPDR;
}

static unsigned char PATTERN_A[] = {0xF0, 0x0F};
static unsigned char PATTERN_B[] = {0xAA, 0x55};
static unsigned char PATTERN_C[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
static unsigned char PATTERN_D[] = {0x77, 0xCC};

static unsigned short counter = 0;
static unsigned char spd = 1;
static unsigned char currPtrn = 1;
static unsigned char i = 0;

enum masterStates{INIT, ACTIVE};
int Master_Tick(int state) {
	
	static unsigned char key = 0;
	static unsigned char isRdy = 0; // Ready flag
	//Transitions
	switch (state) {
		case INIT:
			state = ACTIVE;
			break;
		case ACTIVE:
			state = ACTIVE;
			break;
		default:
			state = INIT;
			break;
	}
	
	//Actions
	switch (state) {
		case INIT:
			
		break;
		case ACTIVE:
			if(isRdy == 1) {
				if(currPtrn == 1) {
					if(i > 1) {
						i = 0;
					}
					SPI_MasterTransmit(PATTERN_A[i]);
				}else if(currPtrn == 2) {
					if(i > 1) {
						i = 0;
					}
					SPI_MasterTransmit(PATTERN_B[i]);
				}else if(currPtrn == 3) {
					if(i > 14) {
						i = 0;
					}
					SPI_MasterTransmit(PATTERN_C[i]);
				}else if(currPtrn == 4) {
					if(i > 1) {
						i = 0;
					}
					SPI_MasterTransmit(PATTERN_D[i]);
				}
				isRdy = 0;
				++i;
			}
		
			if(spd == 1) {
				if(counter >= 40) {
					isRdy = 1;
					counter = 0;
				}
			}else if(spd == 2) {
				if(counter >= 20) {
					isRdy = 1;
					counter = 0;
				}
			}else if(spd == 3) {
				if(counter >= 10) {
					isRdy = 1;
					counter = 0;
				}
			}else if(spd == 4) {
				if(counter >= 5) {
					isRdy = 1;
					counter = 0;
				}
			}else if(spd == 5) {
				if(counter >= 2) {
					isRdy = 1;
					counter = 0;
				}
			}else if(spd == 6) {
				if(counter >= 1) {
					isRdy = 1;
					counter = 0;
				}
			}
			
			// Keypad
			key = GetKeypadKey();

			if(key == 'A') { //First pattern
				currPtrn = 1;
				LCD_Cursor(7);
				LCD_WriteData('1');		
				i = 0;
			}
			else if(key == 'B') { // Second pattern 
				currPtrn = 2;
				LCD_Cursor(7);
				LCD_WriteData('2');
				i = 0;
			}
			else if(key == 'C') { // Third pattern
				currPtrn = 3;
				LCD_Cursor(7);
				LCD_WriteData('3');
				i = 0;
			}
			else if(key == 'D') { // Fourth pattern
				currPtrn = 4;
				LCD_Cursor(7);
				LCD_WriteData('4');
				i = 0;
			}
			else if(key == '1') { // Speed 1 (2s)
				LCD_Cursor(14);
				LCD_WriteData('1');
				spd = 1;
			}
			else if(key == '2') { // Speed 2 (1s)
				LCD_Cursor(14);
				LCD_WriteData('2');
				spd = 2;
			}
			else if(key == '3') { // Speed 3 (500ms)
				LCD_Cursor(14);
				LCD_WriteData('3');
				spd = 3;
			}
			else if(key == '4') { // Speed 4 (250ms)
				LCD_Cursor(14);
				LCD_WriteData('4');
				spd = 4;
			}
			else if(key == '5') { // Speed 5 (100ms)
				LCD_Cursor(14);
				LCD_WriteData('5');
				spd = 5;
			}
			else if(key == '6') { //Speed 6 (50ms)
				LCD_Cursor(14);
				LCD_WriteData('6');
				spd = 6;
			}
			else if(key == '7') { // uC 1
				LCD_Cursor(21);
				LCD_WriteData('1');
			}
			else if(key == '8') { // uC 2
				LCD_Cursor(21);
				LCD_WriteData('2');
			}
			else if(key == '9') { // uC 3
				LCD_Cursor(21);
				LCD_WriteData('3');
			}
			counter++;
		break;
		default:
			//Do nothing
		break;
	}
	return state;
}

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00; // Output, controls LCD's enable/reset 
	DDRC = 0xFF; PORTC = 0x00; // LCD output port
	//DDRD = 0xF0; PORTD = 0x0F; 
	
	tasksNum = 1; // declare number of tasks
	task myTasks[1]; // initialize the task array
	tasks = myTasks; // set the task array
	
	// Tasks
	unsigned char i = 0; 
	tasks[i].state = -1;
	tasks[i].period = 5;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Master_Tick;
	
	TimerSet(5); 
	TimerOn();
	LCD_init();
	LCD_DisplayString(1, "Ptrn: 1 Spd: 1  uC: 1");

	SPI_MasterInit(); 

    while(1) {	
    }
}