#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/* TODO: 
 * 
 */


/*************************  Variable Prototypes ********************************/

/*****Variables for Testing only****/
char timer_flag; // doesnt work when I put in extern for some reason
char wall_flag; //Set to 1 if clicker triggers interrupt


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
