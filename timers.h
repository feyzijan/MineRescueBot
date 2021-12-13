#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000


/* TODO: 
 * Test the accurate operation of custom delay functions
 * Configure Timer0 to time the movement durations in between reading cards
 * Remove temp variables from Timer0 getters and setters
 */


/*************************  Variable Prototypes ********************************/


unsigned int timer0val;

/*************************  Function Prototypes ********************************/


/***********
 * Function to initialise Timer0
 * Used to time buggy movements
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


/***********
 * Function that causes a delay that allows the buggy to complete a 45 degree turn
 * Modify the global friction variable 
 ***********/
void friction_delay_ms(void);


/************
 * Function to generate a delay equal to the inputted number (in ms)
 ************/
void custom_delay_ms(unsigned int delay_time);


/****** Functions for Testing Purposes - Will be removed from final build *****/


/********************************
 * Function to initialise Timer1
 * Used for serial transmission 
 ********************************/
void Timer1_init(void);



#endif
