#include <xc.h>
#include "buttons.h"

void buttonsInit(void){
    //Set all LED pins as output
    // Button RF2 to toggle light on and off for testing
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin
    
    // Button RF3 for calibration routines 
    TRISFbits.TRISF3=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF3=0; //turn off analogue input on pin
}


