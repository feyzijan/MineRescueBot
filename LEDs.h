#ifndef _LEDS_H
#define _LEDS_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/********************  Temporarily Storing the LED/Lamp codes here *********/

/*
//LEDs for debugging:
TRISDbits.TRISD7 = 0;
LATDbits.LATD7 = 0; 

TRISHbits.TRISH3 = 0; 
LATHbits.LATH3 = 0;

 
 * 

//Head Lamps - Not Working
TRISHbits.TRISH1 = 0; 
//ANSELDbits.ANSELD0 = 0
LATHbits.LATH1 = 1;

// Main Beam - Not Working
TRISDbits.TRISD3 = 0; 
LATDbits.LATD3 = 0;

// Brake Lights - Not Working
TRISDbits.TRISD4 = 0; 
LATDbits.LATD4 = 0;

// Right Lamps - Not Working
TRISHbits.TRISH0 = 0; 
LATHbits.LATH0 = 0;

// Left Lamps - Working
TRISFbits.TRISF0 = 0; 
LATFbits.LATF0 = 0;

// BATV-Sense - Not Tested
TRISFbits.TRISF6 = 1; 

// Red Led - Working
TRISGbits.TRISG0 = 0; 
LATGbits.LATG0 = 0;

// Blue Led - Working
TRISEbits.TRISE7 = 0; 
LATEbits.LATE7 = 0;

// Green Led - Working
TRISAbits.TRISA3 = 0; 
LATAbits.LATA3 = 0;

 */

/*
 // Testing if LEDs and Lamps work one at a time
        LATGbits.LATG0 = ! LATGbits.LATG0; // Red
        LATEbits.LATE7 = ! LATEbits.LATE7; // Blue
        LATAbits.LATA3 = ! LATAbits.LATA3; // Green
        
        //Head Lamps - Not Working
        LATHbits.LATH1 = !LATHbits.LATH1;

        // Main Beam - Not Working
        LATDbits.LATD3 = !LATDbits.LATD3;

        // Brake Lights - Not Working
        LATDbits.LATD4 = !LATDbits.LATD4;

        // Right Lamps - Not Working
        LATHbits.LATH0 = !LATHbits.LATH0 ;

        // Left Lamps - Working
        LATFbits.LATF0 = !LATFbits.LATF0;
 
        __delay_ms(250);
 */
#endif