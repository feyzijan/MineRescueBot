#ifndef _color_H
#define _color_H

#include <xc.h>
#include "i2c.h"
#include "LEDsButtons.h"
#include "interrupts.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

/* Color Values
 * 1:Red    5: Pink
 * 2:Green  6: Orange 
 * 3:Blue   7: Light Blue
 * 4:Yellow 8: White
 * 9: Black       
 */

/*************************  Variable Prototypes ********************************/

unsigned int int_low, int_high; // Interrupt thresholds - Calibrated

/*************************  Function Prototypes ********************************/


/***************
 * Function to initialise the colour click module using I2C
 ***************/
void color_click_init(void);


/***********
 * Function to initialise clicker interrupt
 * Uses global variables int_low and int_high as lower, upper thresholds
 ***********/
void color_click_interrupt_init(void);


/*************
 * Function to clear and turn off clicker interrupt 
 *************/
void color_click_interrupt_off(void);

/***********************************************
 * Function to clear the RGBC interrupt in the clicker
 ***********************************************/
void color_int_clear(void);


/**********************************************
 * Function to calibrate interrupt thresholds
 * Calibrate interrupt threshold instructions:
 * 1. Hold blue card up perpendicular in front of buggy with a few cm gap
 * 2. Press the left button (RF3) and wait 1s
 * 3. Place buggy in ambient conditions (no cards/walls), press RF3, and wait 1s
 * 4. Hold black card ~7 cm away from buggy and press the button again, wait 1s
 * 6. Press left button again to end calibration
 ***********************************************/
void interrupt_threshold_calibrate(void);


/****************
 *  Function to write to the colour click module
 *  Input "address" is the register within the colour click to write to
 *	Input "value" is the value that will be written to that address
 ****************/
void color_writetoaddr(char address, char value);


/***************
 * Function to read the channel for a colour
 * Input address is the register address of the low byte of the colour	
 * Returns a 16 bit ADC value representing colour intensity
 ***************/
unsigned int color_read(unsigned char address);



/**********************************************
 *  Function to read all colours and store them in inputted array
 ***********************************************/
void read_All_Colors(unsigned int *writeArray);

/*****************
 * Function to take RGBC channel readings and decide on coluor
 * Returns a number signifying colour
 * Takes readings with LED On and Off, employs normalisation as %clear channel
 * And decides on colours based on %Composition - Details in ReadMe
 ******************/
char decide_color(void);




#endif