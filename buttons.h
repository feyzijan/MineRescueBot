#ifndef _buttons_H
#define _buttons_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/* TODO: 
 * 
 */


// Define all LED pins for ease of reference
#define LeftButton PORTFbits.RF3
#define RightButton PORTFbits.RF2


/*************************  Function Prototypes ********************************/


/************
 * Function to set all LED pins as output and set output to zero
 ************/
void buttonsInit(void);

#endif