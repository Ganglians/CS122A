/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 10/6/2015 14:59:51 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_fwd, SM1_rev } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         B = 0x01;
         SM1_State = SM1_fwd;
         break;
         case SM1_fwd: 
         if (A0 == 1) {
            SM1_State = SM1_rev;
         }
         break;
      case SM1_rev: 
         if (A0 == 0) {
            SM1_State = SM1_fwd;
         }
         break;
      default:
         SM1_State = SM1_fwd;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_fwd:
         if(B7 == 1) {
             B = 0x01;
         }
         
         else {
             B = B << 1;
         }
         break;
      case SM1_rev:
         if(B0 == 1) {
             B = 0x80;
         }
         
         else {
         B = B >> 1;
         }
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 1000; // 1000 ms default
   TimerSet(periodState_machine_1);
   TimerOn();
   
   SM1_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
      while(!SM1_Clk);
      SM1_Clk = 0;
   } // while (1)
} // Main