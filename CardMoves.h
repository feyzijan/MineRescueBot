#ifndef _CARD_MOVES_H
#define _CARD_MOVES_H

#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 64000000

// Variables that will be used to keep track of previous moves and revert back to start
char moves[30];
char move_times[30];
char *pmoves = &moves;
char *pmove_times[30];



/************Function prototypes for color cards*****************************/

/*******************************************************************
 * Function that calls the appropriate move function based on the color
 * And appends the color to a list that can later be reversed
 ******************************************************************/
void pick_move(char color,  char *move_list, struct DC_motor *mL, struct DC_motor *mR);

/********** Red Card: Turn Right  90° *************************/
void red_move(struct DC_motor *mL, struct DC_motor *mR);

/********** Green Card: Turn Left 90° ************************/
void green_move(struct DC_motor *mL, struct DC_motor *mR);

/********** Blue Card: Turn back 180°*********************/
void blue_move(struct DC_motor *mL, struct DC_motor *mR);

/********** Yellow Card: Reverse one square + turn right 90°***/
void yellow_move(struct DC_motor *mL, struct DC_motor *mR);

/********** Yellow Card: Reverse one square + turn left 90° ***/
void pink_move(struct DC_motor *mL, struct DC_motor *mR);

/********** Orange Card: Turn Right 135° s***************/
void orange_move(struct DC_motor *mL, struct DC_motor *mR);

/********** LightBlue Card: Turn Left 135° ****************/
void lightblue_move(struct DC_motor *mL, struct DC_motor *mR);

/**********  White Card: Go back to Start ***************/
/* This function loops backwards through the list of colours stored in move_list 
 * to get back to the start condition */
void white_move(char *move_list, struct DC_motor *mL, struct DC_motor *mR);

/********** Black Card: Maze Wall - do nothing****************/
void black_move(struct DC_motor *mL, struct DC_motor *mR);


#endif
