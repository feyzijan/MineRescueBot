#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/* TODO: 
 * 
 */

// Variable to check readings are taken every second - remove after testing
char timer_flag; 


/**********************************************
 *  Function to initialise all interrupts
 ***********************************************/
void Interrupts_init(void);


/**********************************************
 *  High Priority Interrupt Routine
 ***********************************************/
void __interrupt(high_priority) HighISR();

#endif
