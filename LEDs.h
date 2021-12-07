#ifndef _LEDs_H
#define _LEDs_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/* TODO: 
 * 
 */


// Define all LED pins for ease of reference
#define RedLight LATGbits.LATG0
#define BlueLight LATEbits.LATE7
#define GreenLight LATAbits.LATA3
#define LED1 LATDbits.LATD7 
#define LED2 LATHbits.LATH3
#define HeadLamp LATHbits.LATH1
#define MainBeam LATDbits.LATD3
#define BrakeLight LATDbits.LATD4 
#define LeftLamp LATFbits.LATF0 
#define RightLamp LATHbits.LATH0 


/*************************  Function Prototypes ********************************/


/************
 * Function to set all LED pins as output and set output to zero
 ************/
void LEDsInit(void);


/************
 * Function to toggle front white light with RGB LEDs
 ************/
void LightToggle(void);



/****** Functions for Testing Purposes - Will be removed from final build *****/

/*************
 * Function to test that LEDs work 
 *************/
void LEDTest(void);


#endif