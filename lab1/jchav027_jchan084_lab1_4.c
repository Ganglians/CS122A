/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 10/6/2015 14:43:4 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_wait1, SM1_s1 } SM1_State;

TickFct_State_machine_1() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_wait1;
         break;
      case SM1_wait1:
         if (A != 0x03) {
            SM1_State = SM1_wait1;
         }
         else if (A == 0x03) {
            SM1_State = SM1_s1;
            B = 0x00;
         }
         break;
      case SM1_s1:
         if (A != 0x03) {
            SM1_State = SM1_wait1;
            B = 0x00;
         }
         break;
      default:
         SM1_State = SM1_wait1;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_wait1:
         break;
      case SM1_s1:
         if(B == 0x00) {
           B = 0x01;
         }
         
         else if(B == 0x01) {
           B = 0x03;
         }
         
         else {
           B = B << 1;
         } 
          
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM5_States { SM5_wait2, SM5_s2 } SM5_State;

TickFct_State_machine_5() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM5_State) { // Transitions
      case -1:
         SM5_State = SM5_wait2;
         break;
      case SM5_wait2:
         if (A != 0x09) {
            SM5_State = SM5_wait2;
         }
         else if (A == 0x09) {
            SM5_State = SM5_s2;
            B = 0x00;
         }
         break;
      case SM5_s2:
         if (A != 0x09) {
            SM5_State = SM5_wait2;
            B = 0x00;
         }
         break;
      default:
         SM5_State = SM5_wait2;
      } // Transitions

   switch(SM5_State) { // State actions
      case SM5_wait2:
         break;
      case SM5_s2:
         if(B == 0x00) {
           B = 0x80;
         }
         
         else if(B == 0x80) {
           B = 0x40;
         }
         
         else if(B == 0x40) {
          B = 0x20;
         }
         
         else if(B == 0x20) {
           B = 0x90;
         }
         
         else {
           B = B >> 1;
         }
           
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM6_States { SM6_wait3, SM6_s2 } SM6_State;

TickFct_State_machine_6() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
static int i;
static int vals[] = { 0x39, 0xC6, 0x9C, 0x63 };
   switch(SM6_State) { // Transitions
      case -1:
         SM6_State = SM6_wait3;
         break;
      case SM6_wait3:
         if (A != 0x39) {
            SM6_State = SM6_wait3;
         }
         else if (A == 0x39) {
            SM6_State = SM6_s2;
            B = 0x00;
i = 0;
         }
         break;
      case SM6_s2:
         if (A != 0x39) {
            SM6_State = SM6_wait3;
            B = 0x00;
         }
         break;
      default:
         SM6_State = SM6_wait3;
      } // Transitions

   switch(SM6_State) { // State actions
      case SM6_wait3:
         break;
      case SM6_s2:
         if(i > 3) {
           i = 0;
         }
         
         B = vals[i];
         i ++;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM7_States { SM7_wait4, SM7_s2 } SM7_State;

TickFct_State_machine_7() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
static int i;
static int valss[] = { 0x81, 0x42, 0x24, 0x18, 0x24, 0x42 };
   switch(SM7_State) { // Transitions
      case -1:
         SM7_State = SM7_wait4;
         break;
      case SM7_wait4:
         if ((A == 0x03) || (A == 0x09) || (A == 0x39)) {
            SM7_State = SM7_wait4;
         }
         else if (!((A == 0x03) || (A == 0x09) || (A == 0x39))) {
            SM7_State = SM7_s2;
            i = 0;
B = 0x00;
         }
         break;
      case SM7_s2:
         if ((A == 0x03) || (A == 0x09) || (A == 0x39)) {
            SM7_State = SM7_wait4;
            B = 0x00;
         }
         break;
      default:
         SM7_State = SM7_wait4;
      } // Transitions

   switch(SM7_State) { // State actions
      case SM7_wait4:
         break;
      case SM7_s2:
         if(i > 5) {
           i = 0;
         }
         
         B = valss[i];
         i ++;
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
   SM5_State = -1;
   SM6_State = -1;
   SM7_State = -1;
   while(1) {
      TickFct_State_machine_1();
      TickFct_State_machine_5();
      TickFct_State_machine_6();
      TickFct_State_machine_7();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}