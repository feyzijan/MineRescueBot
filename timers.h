#ifndef _timers_H
#define _timers_H

#include <xc.h>
#include "dc_motor.h"
#include "CardMoves.h"
#include  "Memory.h"

#define _XTAL_FREQ 64000000

/*************************  Function Prototypes ********************************/

/***********
 * Function to initialise Timer0 i 16 bit mode
 * Used to time buggy motion and trigger lost funciton with overflow 
 * CS:Fosc/4, PS:1:16384 -> Overflows in ~67.1 seconds, 1 bit = 1.024ms
 ***********/
void Timer0_init(void);


/***********
 * Function to read TMR0 value and store it in the timer list (in ms)
 ***********/
void getTMR0_in_ms(void);


/************
 * Function to set TMR0 value to 0 
 * This is done after each clicker interrupt so motions can be accurately timed
 ************/
void ResetTMR0(void);


/************
 * Function to generate a delay equal to the inputted number (in ms)
 * Used in TurnLeft, TurnRight, ReverseSquare, and ForwardSquare functions
 * As these need a variable as delay input
 * 
 ************/
void custom_delay_ms(int delay_time);



#endif

