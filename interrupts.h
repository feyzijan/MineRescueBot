#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

char timer_flag; // variable to check readings are taken every second

/**********************************************
 *  Function to initialise all interrupts
 ***********************************************/
void Interrupts_init(void);

/**********************************************
 *  Function to write necessary bytes via I2C to clicker for interrupt
 ***********************************************/
void color_click_interrupt_init(void);

void __interrupt(high_priority) HighISR();


#endif
