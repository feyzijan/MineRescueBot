#ifndef _color_H
#define _color_H

#include <xc.h>


#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

/*TODO: 
 * Complete decide_color() function 
 * Update LED_cross_talk to be a constant
 */


/* Color Values
 * 1:Red    5: Pink
 * 2:Green  6: Orange 
 * 3:Blue   7: Light Blue
 * 4:Yellow 8: White
 * 9:       
 */


/*************************  Variable Prototypes ********************************/

unsigned int int_low, int_high; // Interrupt thresholds

unsigned int red, green, blue, clear;


//Cross talk values
unsigned int LED_cross_talk[4] = {484,268,183,993};

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
void read_All_Colors(void);


/***********************************************
 * Function to clear the RGBC interrupt in the clicker
 ***********************************************/
void color_int_clear(void);

/*****************
 * Function to take RGBC channel readings and return a 
 * a char corresponding to the color
 ******************/
char decide_color(void);


/*****************
 * Test function - we assign color manually
 ******************/
char decide_color_test(void);


/****** Functions for Testing Purposes - Will be removed from final build *****/

/**********************************************
 *  Function to convert colour reading to string
 ***********************************************/
void Color2String(char *ptr, unsigned int *pval);


/**********************************************
 *  Function to send all Color readings over serial port to PC
 ***********************************************/
void SendColorReadings(void);


/**********************************************
 * Function to send back interrupt status at register 0x12 to PC
 ***********************************************/
void get_int_status(void);



/**********************************************
 * Function to calibrate interrupt thresholds
 * 
 ***********************************************/
// Calibrate interrupt threshold instructions:
// 1. Hold blue card up to front of buggy with a few millimeters gap while perpendicular to the floor
// 2. press the left button (RF3)
// 3. Leave blue card in front of buggy for at least a second
void interrupt_threshold_calibrate(void);
#endif