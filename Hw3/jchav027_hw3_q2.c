#include "rims.h"

char width, height, weight;
unsigned char big; // Acts like a boolean 

unsigned char TimerFlag = 0;

void TimerISR() {
  TimerFlag = 1;
}

// Function checks boxes dimensions to see if too big
unsigned char isBig(w, h, we) { // takes width height and weight
    if (w > 4) {
      return 1;
    }

    else if (h > 4) {
      return 1;
    }

    else if ( (h + w) > 7) {
      return 1;
    }

    else if (we > 2) {
      return 1;
    }

    else if ( (we * (w + h)) > 10) {
      return 1;
    }

    // If it's none of these things, the box is not big
    return 0;
}

enum featureStates { idle1 } feature_state;

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
      case idle1: // Do 'idle' work (i.e. Read from PORTA)
        height = (A & 0x07);
        width  = ( (A & 0x38) >> 3);
        weight = ( (A & 0xC0) >> 6);
      break;

      default:
        // Do nothing
      break;
   }
}

enum patternState { idle2 } pattern_state;

patternTick() {
  switch(pattern_state) { // Transitions
    case idle2:
      // Do nothing
    break;

    default:
      pattern_state = idle2;
    break;
  }

  switch(pattern_state) { // Actions
    case idle2: // Do 'idle' work (i.e. Check box dimensions)
      big = isBig(width, height, weight);
    break;

    default: 
      // Do nothing
    break;
  } 
}

enum actuateState { idle3, light, linger } actuate_state;

actuateTick() {
  static unsigned char itters;

    switch(actuate_state) { // Transitions
      case -1:
        actuate_state = idle3;
      break;

      case idle3:
        if (big) {
          actuate_state = light;
          itters = 0;
        }
      break;

      case light:
        if (itters != 5) {
          actuate_state = light;
          itters++;
        }

        else if (itters == 5) {
          actuate_state = linger;
        }
      break;

      case linger:
        if (big) { // Remain while box
          actuate_state = linger;
        }

        else { 
          actuate_state = idle3;
          B0 = 0;
        }
      break;

      default:
        actuate_state = idle3;
      break;
    }

    switch(actuate_state) { // Actions
      case idle3:
        // Do nothing
      break;

      case light:
        B0 = 1;
      break;

      case linger:
        B0 = 1;
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
  actuate_state = -1;

  while(1) {
    featureTick();
    patternTick();
    actuateTick();

    while (!TimerFlag);
    TimerFlag = 0;
  }
}