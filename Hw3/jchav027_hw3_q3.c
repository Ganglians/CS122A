#include "rims.h"

unsigned char noise; 
unsigned char disturbances;
unsigned char panic;

unsigned char TimerFlag = 0;

void TimerISR() {
   TimerFlag = 1;
}

enum featureState { init1, idle1 } feature_state;

featureTick() {
   switch(feature_state) { // Transitions
      case idle1:
         // Do nothing
      break;

      default:
         feature_state = idle1;
      break;
   }

   switch(feature_state) { // Actions
      case idle1:
         noise = A;
      break;

      default: 
         // Do nothing
      break;
   }
}

enum patternState { idle2, loudness } pattern_state;

patternTick() {
   static char tmp_count = 0;

   switch(pattern_state) { // Transitions
      case idle2:
         if (noise >= 99 && !panic) { // Wait for loud noise
            pattern_state = loudness;
            tmp_count = 0;
         }
      break;

      case loudness:
         if (noise >= 99 && tmp_count != 2) {
            tmp_count++;
         }
         else if (noise < 99) {
            // Do nothing
         }
         else if (noise >= 99 && tmp_count == 2) {
            disturbances++;
         }

         if (disturbances == 3) {
            panic = 1;
         }
      break;

      default:
         pattern_state = idle2;
      break;
      }

   switch(pattern_state) { // Actions
      case idle2:
         // Do nothing
      break;

      case loudness:
         // Do nothing
      break;

      default: 
         // Do nothing
      break;
   } 
}

enum countState { idle3 } count_state;

countTick() {
   static char tmp_count = 0;

   switch(count_state) { // Transitions
      case idle3: // Do 'idle' work (i.e. counting and bookeeping)
         if (tmp_count == 20 ) {
            tmp_count = 0;
            disturbances = 0;
         }
         else if (tmp_count != 20) {
            tmp_count++;
         }
      break;

      default:
         count_state = idle3;
      break;
      }

   switch(count_state) { // Actions
      case idle3:

      break;

      default:
         // Do nothing 
      break;
   } 
}

enum actuateState { idle4, toggle } actuate_state;

actuateTick() {
   static char tmp_count = 0;

   switch(actuate_state) { // Transitions
      case idle4:
         if (panic) {
            actuate_state = toggle;
            tmp_count = 0;
         }
      break;

      case toggle:
         if (tmp_count != 10) {
            tmp_count ++;
         }

         else if (tmp_count == 10) {
            actuate_state = idle4;

            // Reset values
            B0 = 0;
            panic = 0;
            disturbances = 0;
         }
      break;

      default:
         actuate_state = idle4;
      break;
      }

   switch(actuate_state) { // Actions
      case idle4:
         // Do nothing
      break;

      case toggle:
         B0 = ~B0;
      break;

      default: 
         // Do nothing
      break;
   } 
}
int main() {
   // Set PORTB to be initially zero
   B = 0; 

   // Set timer
   TimerSet(500);
   TimerOn();

   // Set initial states
   feature_state = -1;
   pattern_state = -1;
   count_state   = -1;
   actuate_state = -1;

   while(1) {
      featureTick();
      patternTick();
      countTick();
      actuateTick();

      while (!TimerFlag);
      TimerFlag = 0;
   }
}