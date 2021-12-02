#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

char timer_flag;

void Interrupts_init(void);
void __interrupt(high_priority) HighISR();
void __interrupt(low_priority) LowISR();

// serial functions used in interrupt



#endif
