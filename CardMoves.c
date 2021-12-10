#include <xc.h>
#include "CardMoves.h"
#include "dc_motor.h"
#include "timers.h"

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

// Reverse one square time
int reverse_time = 100; // this is multiplied with 10ms


/* Pick which move to execute based on the color you decided on
/Before picking store in the timer_list how long the car had to move in a straight
*line since it last stopped
*Then add the function of the read color's '"complement" to the function pointer list
*/
void pick_move(char color, struct DC_motor *mL, struct DC_motor *mR){
    //Read Timer0 value and add to time list
    unsigned int temp_time = getTMR0_in_ms();
    add_timing(temp_time);
    
    if (color == 'w'){
        white_move(mL,mR); // pass move list so you can revert it 
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
            
        } else if (color == 'p'){
            
            pink_move(mL,mR); 
        
        } else if (color == 'o'){
            add_function_ptr(&lightblue_move);
            
            orange_move(mL,mR);
            
        } else if (color == 'l'){
            add_function_ptr(&orange_move);
            
            lightblue_move(mL,mR);
        }
        setTMR0(0);//restart Timer
    }
}


//Turn Right  90?
void red_move(struct DC_motor *mL, struct DC_motor *mR) {
    TurnRight(mL,mR);
    TurnRight(mL,mR);
}


//Turn left 90? 
void green_move(struct DC_motor *mL, struct DC_motor *mR){
    TurnLeft(mL,mR);
    TurnLeft(mL,mR);
}


//Turn back 180?
void blue_move(struct DC_motor *mL, struct DC_motor *mR){
    for(char i=0;i<4;i++) TurnLeft(mL,mR);
}


//Reverse one square + turn right 90?
void yellow_move(struct DC_motor *mL, struct DC_motor *mR){
    move_backward(mL,mR,reverse_time);
    stop(mL,mR);
    TurnRight(mL,mR);
    TurnRight(mL,mR);
}


//Reverse one square + turn left 90?
void pink_move(struct DC_motor *mL, struct DC_motor *mR){
    //reverse
    move_backward(mL,mR,reverse_time);
    stop(mL,mR);
    TurnLeft(mL,mR);
    TurnLeft(mL,mR);
}


// Turn Right 135?
void orange_move(struct DC_motor *mL, struct DC_motor *mR){
    
    for(char i=0;i<3;i++)TurnRight(mL,mR);

}


// Turn Left 135?
void lightblue_move(struct DC_motor *mL, struct DC_motor *mR){
    
    for(char i=0;i<3;i++) TurnLeft(mL,mR);

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
        temp_time = get_timing(); // get the last movement time (index auto decrements)
        
        //Move car forward for specified time and stop
        move_forward(mL,mR,temp_time); 
        stop(mL,mR);
        
        //Execute function in function pointer (index auto decrements)
        temp_func = get_function_ptr();
        temp_func(mL,mR);
        __delay__ms(10); // Just some slack
    }
    stop(mL,mR);
    
    
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