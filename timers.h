#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000


/* TODO: 
 * Test removing timer0val declaration from timers.c 
 */


/*************************  Variable Prototypes ********************************/
unsigned int timer0val;



/*************************  Function Prototypes ********************************/

/***********
 * Function to initialise Timer0
 * Used to time buggy movements
 * Overflows in ~67.1 seconds, 1 bit = 1.024ms
 ***********/
void Timer0_init(void);


/***********
 * Function to read TMR0 value and store it in the timer list (as milisecond value)
 ***********/
void getTMR0_in_ms(void);


/************
 * Set TMR1 value
 * Start at 2^16 - move_time so it overflows in move_time seconds
 ************/
void ResetTMR0(void);

/************
 * Function to generate a delay equal to the inputted number (in ms)
 ************/
void custom_delay_ms(unsigned int delay_time);



#endif

