#ifndef _LEDS_H
#define _LEDS_H

#include <xc.h>

#define _XTAL_FREQ 64000000

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


/* Set all LED pins as output and set output to zero*/
void LEDsInit(void);

/* Toggle front white light with RGB LEDs*/
void LightToggle(void);

/*Test that LEDs work - will not be in final version*/
void LEDTest(void);


#endif