/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 9/29/2015 15:43:53 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_wait, SM1_s2, SM1_s3 } SM1_State;

TickFct_State_machine_1() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
int i;
static int patterns[] = { 0x01, 0x12, 0x80, 0x20, 0x40, 0x10, 0x32, 0x33, 0x22, 0x13 };
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_wait;
         break;
      case SM1_wait:
         if (A7 == 0 && A6 == 1) {
            SM1_State = SM1_s2;
            i = 0;
         }
         else if (A7 != 0 || A6 != 1) {
            SM1_State = SM1_wait;
         }
         break;
      case SM1_s2:
         if (A7 != 0 || A6 !=1) {
            SM1_State = SM1_wait;
         }
         else if (A0 == 1) {
            SM1_State = SM1_s3;
         }
         break;
      case SM1_s3:
         if (A7 != 0 || A6 != 1) {
            SM1_State = SM1_wait;
         }
         else if (A0 == 0) {
            SM1_State = SM1_s2;
         }
         break;
      default:
         SM1_State = SM1_wait;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_wait:
         break;
      case SM1_s2:
         B = patterns[i];
         if(i == 7) {
             i = 0;
         }
         
         else {
             i ++;
         }
         break;
      case SM1_s3:
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

enum SM2_States { SM2_wait, SM2_fwd, SM2_rev } SM2_State;

TickFct_State_machine_2() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM2_State) { // Transitions
      case -1:
         SM2_State = SM2_wait;
         break;
      case SM2_wait:
         if (A7 != 0 || A6 != 0) {
            SM2_State = SM2_wait;
         }
         else if (A7 == 0 && A6 == 0) {
            SM2_State = SM2_fwd;
            B = 0x01;
         }
         break;
      case SM2_fwd:
         if (A0 == 1 && A7 == 0 && A6 == 0) {
            SM2_State = SM2_rev;
         }
         else if (A7 != 0 || A6 !=0) {
            SM2_State = SM2_wait;
         }
         break;
      case SM2_rev:
         if (A0 == 0 && A7 == 0 && A6 == 0) {
            SM2_State = SM2_fwd;
         }
         else if (A7 != 0 || A6 != 0) {
            SM2_State = SM2_wait;
         }
         break;
      default:
         SM2_State = SM2_wait;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_wait:
         break;
      case SM2_fwd:
         if(B7 == 1) {
             B = 0x01;
         }
         
         else {
             B = B << 1;
         }
         break;
      case SM2_rev:
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

enum SM3_States { SM3_wait, SM3_s2 } SM3_State;

TickFct_State_machine_3() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM3_State) { // Transitions
      case -1:
         SM3_State = SM3_wait;
         break;
      case SM3_wait:
         if (A7 != 1 || A6 != 1) {
            SM3_State = SM3_wait;
         }
         else if (A7 == 1 && A6 == 1) {
            SM3_State = SM3_s2;
         }
         break;
      case SM3_s2:
         if (A7 != 1 || A6 != 1) {
            SM3_State = SM3_wait;
         }
         break;
      default:
         SM3_State = SM3_wait;
      } // Transitions

   switch(SM3_State) { // State actions
      case SM3_wait:
         break;
      case SM3_s2:
         B = 0xFF;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}
int main() {
   B = 0; //Init outputs
   TimerSet(1000);
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