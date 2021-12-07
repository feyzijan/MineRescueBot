#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/* TODO: 
 * 
 */


/*************************  Variable Prototypes ********************************/

/*****Variables for Testing only****/
char timer_flag; 



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
