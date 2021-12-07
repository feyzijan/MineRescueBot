#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000


/* TODO: Switch the testing interrupt operation to Timer1 - Add Timer 1 init function
 * Configure Timer0 to time the movement durations in between reading cards
 * Add read and write functions for Timer 0
 * Modify getTMR0 function to return time in miliseconds/10
 */


/*************************  Variable Prototypes ********************************/

extern int friction;


/*************************  Function Prototypes ********************************/


/***********
 * Function to initialise Timer0
 * Used to time buggy movements
 ***********/
void Timer0_init(void);


/***********
 * Function to read TMR0 value
 ***********/
unsigned int getTMR0(void);


/************
 * Set TMR1 value
 * Start at 2^16 - move_time so it overflows in move_time seconds
 ************/
void setTMR0(unsigned int t_start);


/***********
 * Delay long enough for succesful turns
 ***********/
void friction_delay_ms(void);


/************
 * Delay for custom amount of time
 * Used in reverse motion
 ************/
void custom_delay_ms(unsigned int delay_time);



/****** Functions for Testing Purposes - Will be removed from final build *****/

/********************************
 * Function to initialise Timer1
 * Used for serial transmission 
 ********************************/
void Timer1_init(void);


#endif
