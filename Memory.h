#ifndef _Memory_H
#define _Memory_H

#include <xc.h>
#include "dc_motor.h"
#include "timers.h"
#include "interrupts.h"


#define _XTAL_FREQ 64000000



/*************************  Variable Prototypes ********************************/


// Define function pointer type
typedef void (*card_func)(struct DC_motor *, struct DC_motor *);

// Array of functions to call during reverse operation
// Each function call is a "complement" of the color card that is read
card_func funcPtrList[30];
char func_index;


// List of times that we moved forward by
// This is automatically incremented when we call getTMR0()
int timeList[30];
char time_index;

// Both indices update automatically with their getter/setter functions


/*************************  Function Prototypes ********************************/

// Getters and setters for the two arrays


/**********
 * Function to add a function pointer to list of function pointers
 * Post-increments index, sets lost_flag if memory is exceeded
 **********/
void add_function_ptr(card_func func);


/********
 * Function to add a timing to our list, and post-decrement index
 ********/
void add_timing(unsigned int timing);


/**********
 * Function to get a timing from our list (pre-decrements index)
 **********/
 unsigned int get_timing(void);

 
/********
 * Function to get a function pointer from our list(pre-decrements index)
 ********/
 card_func get_function_ptr(void);



#endif