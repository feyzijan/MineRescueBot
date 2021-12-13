#include <xc.h>
#include "CardMoves.h"
#include "dc_motor.h"
#include "timers.h"

/*TODO:
 * 
 *
 * Figure out how to reverse the "reverse one square" stuff
 */

// Global variables take up lot of memory
// data flow in program


//Initialise variabels
int timeList[30];
char time_index = 0;

// Array of functions to call
card_func funcPtrList[30];
char func_index = 0;

// Reverse one square time
//int reverse_time = 100; // this is multiplied with 10ms

char end_motion = 0;



/* Pick which move to execute based on the color you decided on
/Before picking store in the timer_list how long the car had to move in a straight
*line since it last stopped
*Then add the function of the read color's '"complement" to the function pointer list
*/
void pick_move(char color, struct DC_motor *mL, struct DC_motor *mR){
    if (color == 8){ // White
        white_move(mL,mR); // pass move list so you can revert it 
    } else {
        if(color == 1){ // Red
            add_function_ptr(&green_move);
            red_move(mL,mR);
            
        } else if(color == 2){ // Green
            add_function_ptr(&red_move);
            green_move(mL,mR);
            
        } else if(color == 3){ // Blue
            add_function_ptr(&blue_move);
            blue_move(mL,mR);
            
        } else if (color == 4){ // Yellow
            add_function_ptr(reverse_yellow_move); 
            yellow_move(mL,mR);
            
        } else if (color == 5){ // Pink
            add_function_ptr(reverse_pink_move);
            pink_move(mL,mR); 
        
        } else if (color == 6){ // Orange
            add_function_ptr(&lightblue_move);
            orange_move(mL,mR);
            
        } else if (color == 7){ // Light Blue
            add_function_ptr(&orange_move);
            lightblue_move(mL,mR);
        }
        ResetTMR0();//restart Timer
    }
}


//Turn Right  90°
void red_move(struct DC_motor *mL, struct DC_motor *mR) {
    PrepareForTurn(mL,mR);
    TurnRight(mL,mR);
    TurnRight(mL,mR);
}


//Turn left 90° 
void green_move(struct DC_motor *mL, struct DC_motor *mR){
    PrepareForTurn(mL,mR);
    TurnLeft(mL,mR);
    TurnLeft(mL,mR);
}


//Turn back 180°
void blue_move(struct DC_motor *mL, struct DC_motor *mR){
    PrepareForTurn(mL,mR);
    for(char i=0;i<4;i++) TurnLeft(mL,mR);
}



// Turn Right 135°
void orange_move(struct DC_motor *mL, struct DC_motor *mR){
    PrepareForTurn(mL,mR);
    for(char i=0;i<3;i++)TurnRight(mL,mR);
}


// Turn Left 135°
void lightblue_move(struct DC_motor *mL, struct DC_motor *mR){
    PrepareForTurn(mL,mR);
    for(char i=0;i<3;i++) TurnLeft(mL,mR);
}

//Reverse one square + turn right 90°
void yellow_move(struct DC_motor *mL, struct DC_motor *mR){
    reverse_square(mL,mR);
    TurnRight(mL,mR);
    TurnRight(mL,mR);
}



//Reverse one square + turn left 90°
void pink_move(struct DC_motor *mL, struct DC_motor *mR){
    reverse_square(mL,mR);
    TurnLeft(mL,mR);
    TurnLeft(mL,mR);
}


//Turn right 90° + Undo reverse one square
void reverse_yellow_move(struct DC_motor *mL, struct DC_motor *mR){
    TurnRight(mL,mR);
    TurnRight(mL,mR);
    forward_square(mL,mR);
    stop(mL,mR); 
}


// Turn left 90° + Undo reverse one square
void reverse_pink_move(struct DC_motor *mL, struct DC_motor *mR){
    TurnLeft(mL,mR);
    TurnLeft(mL,mR);
    forward_square(mL,mR);
    stop(mL,mR);
}

// Execute reverse instructions
void white_move(struct DC_motor *mL, struct DC_motor *mR)
{
    // First Turn back
    for(char i=0;i<4;i++) TurnLeft(mL,mR);
    
    //Loop through the two lists  - first time then function pointer
    unsigned int temp_time;
    card_func temp_func;
    
    while(time_index >= 0){
    //while(func_index >= 0){
        temp_time = get_timing(); // get the last movement time (index auto decrements)
        
        //Move car forward for specified time and stop
        move_forward(mL,mR,temp_time); 
        stop(mL,mR);
        
        //Execute function in function pointer (index auto decrements)
        temp_func = get_function_ptr();
        temp_func(mL,mR);

        __delay_ms(1000); // Just some slack
    }
    stop(mL,mR); 
    end_motion = 1; // End motion
    LightTest(); // Flash lights to signify end
}

/*************Functions for Function Pointer and Timing arrays****************/


void add_function_ptr (card_func func){
    funcPtrList[func_index++] = func;
}


void add_timing(unsigned int timing){
    timeList[time_index++] = timing;
}

// Make this return the 16bit
unsigned int get_timing(void){
    return timeList[--time_index];
}


card_func get_function_ptr(void){
    return funcPtrList[--func_index];
}
