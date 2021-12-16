#ifndef _LEDs_H
#define _LEDs_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/* TODO: 
 * Rename Lighttest
 * Rename file
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
#define ButtonRF2 !PORTFbits.RF2 // For ease of user Buttons are = 1 when pressed
#define ButtonRF3 !PORTFbits.RF3

/*************************  Function Prototypes ********************************/


/************
 * Function to set all LED pins as output and set output to zero
 * And configure buttons RF2 and RF3
 ************/
void LEDs_buttons_init(void);



/************
 * Function to turn on front white LED
 ************/
void LightOn(void);


/************
 * Function to turn off front white LED
 ************/
void LightOff(void);

/*************
 * Function to test that LEDs work by toggling them on/off twice 
 *************/
void LightTest(void);


#endif