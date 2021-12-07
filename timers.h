#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000


extern int friction;

/* TODO: Switch the testing interrupt operation to Timer1 - Add Timer 1 init function
 * Configure Timer0 to time the movement durations in between reading cards
 * Add read and write functions for Timer 0
 * 
 */


/********************************
 * Function to initialise Timer0
 * Used to trigger interrupts for serial transmission
 ********************************/
void Timer0_init(void);


/********************************
 * Function to initialise Timer1
 * Used to keep track of movement times
 ********************************/
void Timer1_init(void);


/*****Read TMR1 value******/
unsigned int getTMR1(void);


/*****
 * Set TMR1 value
 * Start at 2^16 - move_time so it overflows in move_time seconds
 ******/
void setTMR1(unsigned int t_start);

/****************************
 * Delay long enough for succesful turns
 ****************************/
void friction_delay_ms(void);

/*
 * Delay for custom amount of time
 * Used in reverse motion
 */

void custom_delay_ms(unsigned int delay_time);

#endif
