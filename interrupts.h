#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/* TODO: 
 * 
 */


/*************************  Variable Prototypes ********************************/


/*****Variables for Testing only****/
char lost_flag; // Flag set with Timer0 interrupt - tells buggy to try to go home
char color_flag; // Flag that is set with clicker interrupt - to display color reading with LED


/*************************  Function Prototypes ********************************/


/**********************************************
 *  Function to initialise all interrupts
 ***********************************************/
void Interrupts_init(void);


/**********************************************
 *  High Priority Interrupt Routine
 ***********************************************/
void __interrupt(high_priority) HighISR();

#endif

