#ifndef _color_H
#define _color_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

/* Variables to use later on
// List of RGBC values and addresses
unsigned int RGBC_val[] = {0,0,0,0};
unsigned char RGBC_reg[] = {0x16, 0x18, 0x1A, 0x14};

unsigned int *pval = RGBC_val;
unsigned char *preg = RGBC_reg;
 * 
 * char colorstring[8]; // string size should be 2 more than string itself
 * 
 * 
//
 */

unsigned int red;
unsigned int green;
unsigned int blue;
unsigned int clear;

/********************************************//**
 *  Function to initialise the colour click module using I2C
 ***********************************************/
void color_click_init(void);

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

/********************************************//**
 *  Function to convert colour reading to string
 ***********************************************/
void Color2String(char *ptr, unsigned int *pval);
#endif

void read_All_Colors(void);

void SendColorReadings(void);