/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 10/1/2015 14:26:29 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_s1, SM1_wait2, SM1_off } SM1_State;

TickFct_State_machine_1() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_s1;
         break;
      case SM1_s1:
         if (A1 == 0) {
            SM1_State = SM1_wait2;
         }
         else if (A1 == 1) {
            SM1_State = SM1_off;
         }
         break;
      case SM1_wait2:
         if (A1 == 0) {
            SM1_State = SM1_s1;
         }
         else if (A1 == 1) {
            SM1_State = SM1_off;
         }
         break;
      case SM1_off:
         if (A1 == 0) {
            SM1_State = SM1_s1;
         }
         break;
      default:
         SM1_State = SM1_s1;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_s1:
         B = 0x55;
         break;
      case SM1_wait2:
         break;
      case SM1_off:
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM2_States { SM2_wait1, SM2_s3, SM2_off } SM2_State;

TickFct_State_machine_2() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM2_State) { // Transitions
      case -1:
         SM2_State = SM2_wait1;
         break;
      case SM2_wait1:
         if (A1 == 0) {
            SM2_State = SM2_s3;
         }
         else if (A1 == 1) {
            SM2_State = SM2_off;
         }
         break;
      case SM2_s3:
         if (A1 == 0) {
            SM2_State = SM2_wait1;
         }
         else if (A1 == 1) {
            SM2_State = SM2_off;
         }
         break;
      case SM2_off:
         if (A1 == 0) {
            SM2_State = SM2_wait1;
         }
         break;
      default:
         SM2_State = SM2_wait1;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_wait1:
         break;
      case SM2_s3:
         B = 0xAA;
         break;
      case SM2_off:
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM3_States { SM3_s1, SM3_s2 } SM3_State;

TickFct_State_machine_3() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM3_State) { // Transitions
      case -1:
         SM3_State = SM3_s1;
         break;
      case SM3_s1:
         if (A1 == 1) {
            SM3_State = SM3_s2;
         }
         break;
      case SM3_s2:
         if (A1 == 0) {
            SM3_State = SM3_s1;
         }
         break;
      default:
         SM3_State = SM3_s1;
      } // Transitions

   switch(SM3_State) { // State actions
      case SM3_s1:
         break;
      case SM3_s2:
         B = 0x00;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}
int main() {
   B = 0; //Init outputs
   TimerSet(500);
   TimerOn();
   SM1_State = -1;
   SM2_State = -1;
   SM3_State = -1;
   while(1) {
      TickFct_State_machine_1();
      TickFct_State_machine_2();
      TickFct_State_machine_3();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}