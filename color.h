#ifndef _color_H
#define _color_H

#include <xc.h>
#include "i2c.h"
#include "LEDsButtons.h"
#include "interrupts.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

/* TODO:  
 * Use cross talk in function and not as a global variable
 * 
 ******************************************************************************/


/* Color Values
 * 1:Red    5: Pink
 * 2:Green  6: Orange 
 * 3:Blue   7: Light Blue
 * 4:Yellow 8: White
 * 9: Black       
 */


/*************************  Variable Prototypes ********************************/

unsigned int int_low, int_high; // Interrupt thresholds
//Cross talk values (pre-calculated) - Clear Red Green Blue
// These values are constant (for 1x Gain and 42 Integration Cycles )
int LED_cross_talk[4] = {993 ,484 ,268,183}; //

/*************************  Function Prototypes ********************************/


/********************************************//**
 *  Function to initialise the colour click module using I2C
 ***********************************************/
void color_click_init(void);

/**********************************************
 *  Function to write necessary bytes via I2C to clicker for interrupt
 ***********************************************/
void color_click_interrupt_init(void);


/**********************************************
 *  Function to turn off clicker interrupt 
 ***********************************************/
void color_click_interrupt_off(void);


/***********************************************
 * Function to clear the RGBC interrupt in the clicker
 ***********************************************/
void color_int_clear(void);

/********************************************//**
 *  Function to write to the colour click module
 *  address is the register within the colour click to write to
 *	value is the value that will be written to that address
 ***********************************************/
void color_writetoaddr(char address, char value);


/********************************************//**
 *  Function to read the red channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read(unsigned char address);


/**********************************************
 *  Function that calls color_read for all four RGBC channels
 ***********************************************/
//void read_All_Colors(void);


/*****************
 * Function to take RGBC channel readings and return a 
 * a char corresponding to the color
 ******************/
char decide_color(void);


/**********************************************
 *  Function that calls color_read for all four RGBC channels
 ***********************************************/
void read_All_Colors(unsigned int *writeArray);



/**********************************************
 * Function to calibrate interrupt thresholds
// Calibrate interrupt threshold instructions:
    // 1. Hold blue card up to front of buggy with a few millimeters gap while perpendicular to the floor
    // 2. press the left button (RF3)
    // 3. Leave blue card in front of buggy for at least a second
    // 4. Remove blue card and press button and wait for at least one second
    // 5. Add black card approximately 7 cm away from the front of the buggy and press the button again, wait for one second
    // 6. Remove the black card
    // 7. Place buggy at the start point of the tracking course and press left button again to end calibration
 ***********************************************/
void interrupt_threshold_calibrate(void);


#endif