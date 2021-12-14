#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000


/* TODO: 
 * 
 */


/*************************  Variable Prototypes ********************************/
unsigned int timer0val;



/*************************  Function Prototypes ********************************/
//******** Timer0 Functions

/***********
 * Function to initialise Timer0
 * Used to time buggy movements
 * Overflows in 134 seconds, 1 bit = 2.048ms
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

