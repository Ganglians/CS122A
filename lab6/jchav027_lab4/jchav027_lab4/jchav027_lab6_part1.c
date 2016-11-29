/* Coder: Juan Chavez
 * Date:  10.16.2015
 * Class: CS122A 
 * Lab 6 Part 1
 * Description: Design a system where the 10-bit binary result from an A2D conversion is displayed on a bank of 10 LEDs. 
 * Use the Joystick to adjust the supplied voltage to PA0.
 */

#include <avr/io.h>
#include "scheduler.h"
#include "timer.h"
#include "bit.h"

unsigned char GCD_Period = 5;

void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//     analog to digital conversions.
}

void transmit_data(unsigned char data) {
	int i;
	for (i = 7; i >= 0 ; --i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x04;
	}
	// set RCLK = 1. Rising edge copies data from the “Shift” register to the “Storage” register
	PORTC |= 0x02;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //Set port A's 8 pins as inputs
	//DDRA = 0x0F; PORTA = 0xF0; //Set first half of A's ports as inputs and second half as outputs
	//DDRC = 0xFF; PORTC = 0x00; //Set port C's 8 pins as ops
	DDRB = 0xFF; PORTB = 0x00; //Set port B's 8 pins as ops
	DDRD = 0xFF; PORTD = 0x00; //Set port D's 8 pins as ops
	
// 	TimerSet(GCD_Period);
// 	TimerOn();
//  
//  	part1.elapsedTime = part1.period;
//  	part1.period = GCD_Period;
//  	part1.state = -1;
//  	part1.TickFct = &part1TickFunction;
// 	 
	//op = 0xFF;
	A2D_init();

	//The 10 bits are truncated (hence tbits) last two bits found by shifting (and storing the shift in mbits)
	unsigned char mbits = 0;//, tbits = 0;	
	char b8 = 0, b9 = 0;
    while(1) {		
		
		PORTB = ADC;
		mbits = ADC >> 2;
		b8 = GetBit(mbits, 0);
		b9 = GetBit(mbits, 1);
		PORTD = SetBit(PORTD, 5, b8);
		PORTD = SetBit(PORTD, 6, b9);
    }
}