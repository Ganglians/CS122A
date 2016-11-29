/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 9/29/2015 18:44:5 PST
*/

#include "rims.h"

int i;
static int patterns[] = { 0x01, 0x12, 0x80, 0x20, 0x40, 0x10, 0x32, 0x33, 0x22, 0x13 };
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_fwd, SM1_rev } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         i = 0;
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
         B = patterns[i];
         if(i == 7) {
             i = 0;
         }
         
         else {
             i ++;
         }
         break;
      case SM1_rev:
         B = patterns[i];
         if(i == 0) {
             i = 7;
         }
         
         else {
             i --;
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