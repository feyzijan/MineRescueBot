#ifndef _CARD_MOVES_H
#define _CARD_MOVES_H

#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 64000000



/*************************  Variable Prototypes ********************************/


// Define function pointer type
typedef void (*card_func)(struct DC_motor *, struct DC_motor *);


// List of times that we moved forward by
int timeList[30];
char time_index;

// Array of functions to call
card_func funcPtrList[30];
char func_index;

char end_motion; //signifies motion has ended


/*************************  Function Prototypes ********************************/



//***************** Functions for color cards

 
/*******************************************************************
 * Function that calls the appropriate move function based on the color
 * And appends the color to a list that can later be reversed
 ******************************************************************/
void pick_move(char color, struct DC_motor *mL, struct DC_motor *mR);


/********** Red Card: Turn Right  90° *************************/
void red_move(struct DC_motor *mL, struct DC_motor *mR);


/********** Green Card: Turn Left 90° ************************/
void green_move(struct DC_motor *mL, struct DC_motor *mR);


/********** Blue Card: Turn back 180°*********************/
void blue_move(struct DC_motor *mL, struct DC_motor *mR);


/********** Yellow Card: Reverse one square + turn right 90°***/
void yellow_move(struct DC_motor *mL, struct DC_motor *mR);


/********** Pink Card: Reverse one square + turn left 90° ***/
void pink_move(struct DC_motor *mL, struct DC_motor *mR);


/********** Reverse Yellow Card: turn right 90° + Move Forward one square***/
void reverse_yellow_move(struct DC_motor *mL, struct DC_motor *mR);

/********** Reverse Pink Card: turn left 90° + Move Forward one square **/
void reverse_pink_move(struct DC_motor *mL, struct DC_motor *mR);

/********** Orange Card: Turn Right 135° s***************/
void orange_move(struct DC_motor *mL, struct DC_motor *mR);


/********** LightBlue Card: Turn Left 135° ****************/
void lightblue_move(struct DC_motor *mL, struct DC_motor *mR);

/**********  White Card: Go back to Start ***************/
/* This function loops backwards through the list of colours stored in move_list 
 * to get back to the start condition */
void white_move(struct DC_motor *mL, struct DC_motor *mR);


//***************** Getters and setters for the two arrays********************//


/**********
 *  Add a function pointer to our list of function pointers, increment index
 **********/
void add_function_ptr(card_func func);


/********
 * Add a timing to our list, decrement index
 ********/
void add_timing(unsigned int timing);


/**********
 * Get a timing from our list, decrement index
 **********/
 unsigned int get_timing(void);

 
/********
 * Get a function pointer from our list, decrement index
 ********/
 card_func get_function_ptr(void);



#endif
