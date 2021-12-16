#ifndef _CARD_MOVES_H
#define _CARD_MOVES_H

#include <xc.h>
#include "dc_motor.h"
#include "timers.h"
#include "interrupts.h"
#include  "Memory.h"


#define _XTAL_FREQ 64000000

/*************************  Function Prototypes ********************************/

// Functions for coluor cards

/*******************************************************************
 * Function that calls the appropriate move function based on the colour
 * Adds the "complement" of the color function it will call to memory
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
/* This function loops backwards through timeList and funcPtrList sequentially
 * First turns buggy 180, then reads times and functions from the end of the lists
 * Moves forward by the time logged in the timeList, then performs function in the
 * Function list to get back to the starting point
 */
void white_move(struct DC_motor *mL, struct DC_motor *mR);

#endif