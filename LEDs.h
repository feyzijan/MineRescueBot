#ifndef _LEDS_H
#define _LEDS_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/********************  Temporarily Storing the LED/Lamp codes here *********/

/*
//LEDs for debugging:
LATDbits.LATD7 = LATHbits.LATH3 = 1; // set initial value
TRISDbits.TRISD7 = TRISHbits.TRISH3 = 0;  //set pins as output (0)

//Head Lamps - Not Working
TRISDbits.TRISD0 = 0; 
LATDbits.LATD0 = 1;

// Main Beam - Not Working
TRISCbits.TRISC5 = 0; 
LATCbits.LATC5 = 0;

// Brake Lights - Not Working
TRISAbits.TRISA0 = 0; 
LATAbits.LATA0 = 0;

// Right Lamps - Not Working
TRISGbits.TRISG3 = 0; 
LATGbits.LATG3 = 0;

// Left Lamps - Working
TRISFbits.TRISF0 = 0; 
LATFbits.LATF0 = 0;

// BATV-Sense - Not Tested
TRISFbits.TRISF6 = 1; 

// Red Led - Working
TRISGbits.TRISG1 = 0; 
LATGbits.LATG1 = 0;

// Blue Led - Working
TRISFbits.TRISF7 = 0; 
LATFbits.LATF7 = 0;

// Green Led - Working
TRISAbits.TRISA4 = 0; 
LATAbits.LATA4 = 0;

 */

/*
 // Testing if LEDs and Lamps work one at a time
        LATGbits.LATG1 = ! LATGbits.LATG1; // Red
        LATAbits.LATA4 = ! LATAbits.LATA4; // Green
        LATFbits.LATF7 = ! LATFbits.LATF7; // Blue
        
        LATDbits.LATD0 = !LATDbits.LATD0; // Head Lamps
        LATAbits.LATA0 = !LATAbits.LATA0; // Brake Lamps
        LATFbits.LATF0 = !LATFbits.LATF0; // Left Lamps
        LATGbits.LATG3 = !LATGbits.LATG3; // Right Lamps
        LATCbits.LATC5 = !LATCbits.LATC5; // Main Beam
        __delay_ms(250);
 */
#endif