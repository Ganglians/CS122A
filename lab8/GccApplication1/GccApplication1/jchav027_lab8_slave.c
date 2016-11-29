/* Name:  Juan Chavez
 * Email: jchav027@ucr.edu
 * Date:  10.29.2015
 * Class: CS122A
 * Lab:   8 (Slave)
 * Description:
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "keypad.h"
#include "bit.h"
#include "lcd.h"
#include "scheduler.h"

unsigned char receivedData;
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


enum slaveStates{INIT, ACTIVE};
int Slave_Tick(int state) {
	static unsigned char i = 0;
	//Transitions
	switch (state) {
		case -1:
			state = INIT;
		break;
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
		case -1:
			// Do nothing
		break;
		case INIT:
			// Do nothing
		break;
		case ACTIVE:
			PORTD = receivedData;
		break;
		default:
			// Do nothing
		break;
	}
	return state;
}

int main(void) {
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	tasksNum = 1; // declare number of tasks
	task myTasks[1]; // initialize the task array
	tasks = myTasks; // set the task array
	
	//Define tasks
	unsigned char i = 0; // task counter
	tasks[i].state = -1;
	tasks[i].period = 2;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Slave_Tick;
	
	TimerSet(2); // value set should be GCD of all tasks
	TimerOn();

	SPI_ServantInit();
	
    while(1) {
    }
}