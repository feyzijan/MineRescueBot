#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>
#include "i2c.h"
#include "color.h"
#include "LEDsButtons.h"
#include "timers.h"

#define _XTAL_FREQ 64000000


/*************************  Variable Prototypes ********************************/

char lost_flag; // Flag set with Timer0 interrupt - tells buggy to try to go home
char color_flag; // Flag set with clicker interrupt - tells main to read color

/*************************  Function Prototypes ********************************/

/************
 * Function to initialise all interrupts
 * Enables clicker and Timer0 interrupts
*************/
void Interrupts_init(void);


/************
 * High priority interrupt service routine for:
 * Clicker RGBC Clear Channel Interrupt: Triggered when a color card is detected
 * Timer0 interrupt:  Triggered after not having read a card in 67 seconds
*************/
void __interrupt(high_priority) HighISR();

#endif

