#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/* TODO: Switch the testing interrupt operation to Timer1 - Add Timer 1 init function
 * Configure Timer0 to time the movement durations in between reading cards
 * Add read and write functions for Timer 0
 * 
 */


/********************************
 * Function to initialise Timer0
 ********************************/
void Timer0_init(void);



#endif
