#include <xc.h>
#include "CardMoves.h"
#include "dc_motor.h"

/*TODO:
 * Check that func_index and time_index are updated globally
 * Check that the movement_durations and funcPtrList are updated globally
 * Figure out how to reverse the "reverse one square" stuff
 */

//Initialise variabels
int timeList[30];
unsigned char time_index = 0;

// Array of functions to call
card_func funcPtrList[30];
unsigned char func_index = 0;



/* Pick which move to execute based on the color you decided on
/Before picking store in the timer_list how long the car had to move in a straight
*line since it last stopped
*Then add the function of the read color's '"complement" to the function pointer list
*/
void pick_move(char color, struct DC_motor *mL, struct DC_motor *mR){
    
    if (color == 'w'){
        //white_move(mL,mR, move_list); // pass move list so you can revert it 
    } else {
        if(color == 'r'){
            add_function_ptr(&green_move);
            red_move(mL,mR);
        } else if(color == 'g'){
            add_function_ptr(&red_move);
            green_move(mL,mR);
        } else if(color == 'b'){
            add_function_ptr(&blue_move);
            blue_move(mL,mR);
        } else if (color == 'y'){
            yellow_move(mL,mR);
        } else if (color == 'o'){
            add_function_ptr(&lightblue_move);
            orange_move(mL,mR);
        } else if (color == 'l'){
            add_function_ptr(&orange_move);
            lightblue_move(mL,mR);
        }
    }
}


/********** Red Card: Turn Right  90° *************************/
void red_move(struct DC_motor *mL, struct DC_motor *mR) {
    TurnRight(mL,mR);
    
}

/********** Green Card: Turn Left 90° ************************/
void green_move(struct DC_motor *mL, struct DC_motor *mR){
    TurnLeft(mL,mR);
}

/********** Blue Card: Turn back 180°*********************/
void blue_move(struct DC_motor *mL, struct DC_motor *mR){
    TurnLeft(mL,mR);
    TurnLeft(mL,mR);
}

/********** Yellow Card: Reverse one square + turn right 90°***/
void yellow_move(struct DC_motor *mL, struct DC_motor *mR){

    TurnRight(mL,mR);
}

/********** Yellow Card: Reverse one square + turn left 90° ***/
void pink_move(struct DC_motor *mL, struct DC_motor *mR){
    //reverse
    TurnLeft(mL,mR);
}

/********** Orange Card: Turn Right 135° s***************/
void orange_move(struct DC_motor *mL, struct DC_motor *mR){

    TurnRight(mL,mR);

}


/********** LightBlue Card: Turn Left 135° ****************/
void lightblue_move(struct DC_motor *mL, struct DC_motor *mR){
    TurnLeft(mL, mR);

}


/********** White Card: Finish - execute reverse instructions ***************/
void white_move(struct DC_motor *mL, struct DC_motor *mR)
{
 //Loop through the two lists  - first function pointers than times
    
    
}

/*************Functions for Function Pointer and Timing arrays****************/


void add_function_ptr (card_func func){
    funcPtrList[func_index] = func;
    func_index ++; 
            
}


void add_timing(unsigned int timing){
    timeList[time_index] = timing;
    time_index ++; //CHECK THAT THIS IS UPDATED GLOBALLY
}

// Make this return the 16bit
unsigned int get_timing(void){
    
    return timeList[time_index--];
}


card_func get_function_ptr(void){
    return funcPtrList[func_index--];
}
