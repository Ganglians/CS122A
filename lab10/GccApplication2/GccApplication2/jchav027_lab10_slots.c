/* Name:  Juan Chavez
 * Email: jchav027@ucr.edu
 * Date:  11.16.2015
 * Class: CS122A
 * Lab:   10
 * Description: Custom lab
 */

////////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdlib.h> // srand(), rand()
#include <math.h>

#include "scheduler.h"
#include "usart_ATmega1284.h"

///////////////////////////////////////////////////////////////////////////////
//#define LEVER (~PINB & 0x01)

// Symbols
// Degrees 		Symbol 		Score
// 0-60			BELL        50
// 61-120		Seven       100
// 121-180		Orange      60
// 181-240		Bar         30
// 241-300		Cherry      90
// 301-359		Lemon       20

// Motor states
#define M_STATES 7

// Max value randomizer can get to
#define RMAX 5

// Timer variable
static unsigned char GCD_Period = 1;

// USART signal variable
unsigned char LEVER;

// EEPROM variables that keep track of slot positioning
//uint16_t EEMEM D1 = 0;
//uint16_t EEMEM D2 = 0;
//uint16_t EEMEM D3 = 0;
unsigned int D1 = 0, D2 = 0, D3 = 0;

unsigned int T1, T2, T3; // Symbol type variables

// Signifies when a specific motor is done spinning
unsigned char Done1, Done2, Done3; 

 // Number of degrees to rotate
static int deg1, deg2, deg3;

// Testing values
int I = 0, J = 0, K = 0;
// int test1[] = {30, 182, 132, 30};
// int test2[] = {187, 5, 138, 40};
// int test3[] = {310, 245, 120, 80};
int test1[] = {140, 55, 55};
int test2[] = {70, 130, 55};
int test3[] = {195, 0, 55};

///////////////////////////////////////////////////////////////////////////////
// Helper functions / Structures
int degConv(int d) { // Converts the degree entered into the phase equivalent (D / 5.625) * 64
	return ((d / 5.625) * 64);
}

// Takes in the degree value and returns numerical value for that symbol
int typeSelect(int d) {
	if(d >= 0 && d <= 60) { 	     // BELL
		return 50;
	}
	else if(d >= 61 && d <= 120) {   // SEVEN
		return 100;
	}	
	else if(d >= 121 && d <= 180) {  // ORANGE 
		return 60;
	}
	else if(d >= 181 && d <= 240) {  // BAR
		return 30;
	}
	else if(d >= 241 && d <= 300) {  // CHERRY
		return 90;
	}
	else if(d >= 301 && d <= 359) {  // LEMON
		return 20;
	}
	else {                           // (FAILSAFE)
		return 50;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Leader function-------------------------------------------------------------
// Polls for signal and lets other SM's know that they need to spin and to what degree
enum leaderState {INIT1, IDLE1, SIGNAL_ON, SIGNAL_OFF ,STANDBY, DONE} leader_state;

int LEADER_Tick() {
	switch(leader_state) { // Transitions
		case INIT1:
			leader_state = IDLE1;
		break;

		case IDLE1:
			if(USART_HasReceived(0)) { // Poll for signal
				leader_state = SIGNAL_ON;
			}
		break;

		case SIGNAL_ON:
			leader_state = SIGNAL_OFF;
		break;

		case SIGNAL_OFF:
			leader_state = STANDBY;
		break;

		case STANDBY:
			if(Done1 && Done2 && Done3) {
				//D1 += (deg1) % 360;
				//D2 += (deg2) % 360;
				//D3 += (deg3) % 360;
				leader_state = DONE;
			}
		break;

		case DONE:
			leader_state = IDLE1;
		break;

		default:
			leader_state = INIT1;
		break;
	}
	switch(leader_state) { // Actions
		case INIT1:

		break;

		case IDLE1:
			// Do nothing
		break;

		case SIGNAL_ON:
			LEVER = USART_Receive(0); // Changing lever status initiates motors
			USART_Flush(0);

			// Signify that none of the motors are done spinning
			Done1 = 0;
			Done2 = 0;
			Done3 = 0;
		break;

		case SIGNAL_OFF:
			LEVER = 0;
		break;

		case STANDBY:
			// Do nothing
		break;

		case DONE:
			if((T1 == T2) && (T2 == T3)) { // If a match is found
				USART_Send(T1, 1);
			}
			leader_state = IDLE1;				
		break;

		default:
			// Do nothing
		break;		
	}
}

//////////////////////////////////////////////////////////////////////////////
// Stepper motor tick functions
// Stepper motor variables
unsigned char motorPhase[] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09}; // A, AB, B, BC, C, CD, D
int i, j, k; // Iterators for the motor phases
int p1, p2, p3; // Keeps track of number of phases motor has gone through
unsigned char B1, B2, B3; // Signifies whether or not SM's should go into spin state
enum motorState {INIT2, IDLE2, CLOCKWISE} motor1_state, motor2_state, motor3_state;
// static int numDegrees;
static int num_phases1, num_phases2, num_phases3; // Number of phases to rotate (D/5.625) * 64
// First motor's tick function-------------------------------------------------
int MOTOR1_Tick() {
	switch(motor1_state) { // Transitions
		case INIT2:
			B1 = 0;
			p1 = 0;
			i  = 0;
			if(I > 2) {
				deg1 = rand() % 360;
			}
			else {
				deg1 = test1[I];
				++ I;
			}
			D1 = (D1 + deg1) % 360;
			num_phases1 = degConv(deg1);
 			motor1_state = IDLE2;
		break;

		case IDLE2:
			if(LEVER) {
				B1 = LEVER;
			}
			if(B1) {
				motor1_state = CLOCKWISE;
			}
			else {
				// Do nothing
			}
		break;

		case CLOCKWISE:
			if( p1 >= num_phases1) { // Done spinning
				Done1 = 1;
				T1 = typeSelect(D1);
				motor1_state = INIT2;
			}
		break;

		default:
			motor1_state = INIT2;
		break;
	}
	switch(motor1_state) { // Actions
		case INIT2:

		break;

		case IDLE2:
			// Do nothing
		break;
 
		case CLOCKWISE:
			if(i > 6) { // Looping back the motor phase
				i = 0;
			}
			PORTA = (PORTA & 0xF0); // Clear lower nibble
			PORTA = (PORTA | motorPhase[i]); // Set lower nibble to phase value

			++ i;
			++ p1;
		break;
		
		default:
			// Do nothing
		break;
	}
}

// Second motor's tick function------------------------------------------------
// Shifted motorPhase for use in upper nibble of PORTA and PORTD
unsigned char motorPhase2[] = {0x10, 0x30, 0x20, 0x60, 0x40, 0xC0, 0x80, 0x90}; // A, AB, B, BC, C, CD, D

int MOTOR2_Tick() {
	switch(motor2_state) { // Transitions
		case INIT2:
			B2 = 0;
			p2 = 0;
			j = 0;
			//deg2 = 30;
			if(J > 2) {
				deg2 = rand() % 360;
			}
			else {
				deg2 = test2[J];
				++ J;
			}
			D2 = (D2 + deg2) % 360;
			num_phases2 = degConv(deg2);
			motor2_state = IDLE2;
		break;

		case IDLE2:
			if(LEVER) {
				B2 = LEVER;
			}
			if(B2) {
				motor2_state = CLOCKWISE;
			}
			else {
				// Do nothing
			}
		break;

		case CLOCKWISE:
			if( p2 >= num_phases2) { // Done spinning
				Done2 = 1;
				T2 = typeSelect(D2);
				motor2_state = INIT2;
			}
		break;

		default:
			motor2_state = INIT2;
		break;
	}
	switch(motor2_state) { // Actions
		case INIT2:

		break;

		case IDLE2:
			// Do nothing
		break;

		case CLOCKWISE:
			if(j > 6) { // Looping back the motor phase
				j = 0;
			}

			PORTA = (PORTA & 0x0F); // Clear upper nibble			
			PORTA =  (PORTA | motorPhase2[j]);

			++ j;
			++ p2;
		break;
		
		default:
			// Do nothing
		break;
	}
}

// Third motor's tick function-------------------------------------------------
int MOTOR3_Tick() {
	switch(motor3_state) { // Transitions
		case INIT2:
			B3 = 0;
			p3 = 0;
			k = 0;
			//deg3 = 30;
			if(K > 2) {
				deg3 = rand() % 360;
			}
			else {
				deg3 = test3[K];
				++ K;
			}
			D3 = (D3 + deg3) % 360;
			num_phases3 = degConv(deg3);
			motor3_state = IDLE2;
		break;

		case IDLE2:
			if(LEVER) {
				B3 = LEVER;
			}

			if(B3) {
				//k = M_STATES;
				motor3_state = CLOCKWISE;
			}

			else {
				// Do nothing
			}
		break;

		case CLOCKWISE:
			if( p3 >= num_phases3) { // Done spinning
				Done3 = 1;
				T3 = typeSelect(D3);
				motor3_state = INIT2;
			}
		break;

		default:
			motor3_state = INIT2;
		break;
	}
	switch(motor3_state) { // Actions
		case INIT2:
	
		break;

		case IDLE2:
			// Do nothing
		break;

		case CLOCKWISE:
			if(k > 6) { // Looping back the motor phase
				k = 0;
			}
			
			PORTD = (PORTD & 0x0F); // Clear upper nibble			
			PORTD =  (PORTD | motorPhase2[k]);

			++ k;
			++ p3;
		break;
		
		default:
			// Do nothing
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Main function
int main(void) {
	DDRA = 0xFF; PORTA = 0x00; // A outputs motor phases
	DDRB = 0x00; PORTB = 0xFF; // B gets button input
	//DDRC = 0xFF; PORTC = 0x00; // C outputs motor phases
	DDRD = 0xFF; PORTD = 0x00; // D outputs motor phases

	// Initialize UART 
	initUSART(0); // Used to receive data
	initUSART(1); // Used to send data

	// Initialize rand
	srand(7);

	// Scheduler task setup
	tasksNum = 4;    		   // Declare number of tasks
	task myTasks[tasksNum];    // Initialize the task array
	tasks = myTasks; 		   // Set the task array

	// Task initialization
	unsigned char _i = 0;

	tasks[_i].state = -1;
	tasks[_i].period = 2;
	tasks[_i].elapsedTime = tasks[_i].period;
	tasks[_i].TickFct = &LEADER_Tick;
	
	++ _i;

	tasks[_i].state = -1;
	tasks[_i].period = 2;
	tasks[_i].elapsedTime = tasks[_i].period;
	tasks[_i].TickFct = &MOTOR1_Tick;

	++ _i;

	tasks[_i].state = -1;
	tasks[_i].period = 2;
	tasks[_i].elapsedTime = tasks[_i].period;
	tasks[_i].TickFct = &MOTOR2_Tick;

	++ _i;

	tasks[_i].state = -1;
	tasks[_i].period = 2;
	tasks[_i].elapsedTime = tasks[_i].period;
	tasks[_i].TickFct = &MOTOR3_Tick;

	// Initialize timer
	TimerSet(GCD_Period); 
	TimerOn();

    while (1) {

    }
}