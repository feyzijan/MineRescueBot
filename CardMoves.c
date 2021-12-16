#include <xc.h>
#include "CardMoves.h"


void pick_move(char color, struct DC_motor *mL, struct DC_motor *mR){
    if (color == 8 ){ // White 
        white_move(mL,mR);  
    } else if (color == 9) { // Black
        LightsToggle(); // Signify it hit a wall
        white_move(mL,mR); // Try to go back gome
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
    }
}


// Turn Right  90°
void red_move(struct DC_motor *mL, struct DC_motor *mR) {
    PrepareForTurn(mL,mR);
    TurnRight(mL,mR);
    TurnRight(mL,mR);
}


// Turn left 90° 
void green_move(struct DC_motor *mL, struct DC_motor *mR){
    PrepareForTurn(mL,mR);
    TurnLeft(mL,mR);
    TurnLeft(mL,mR);
}


// Turn back 180°
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


// Reverse one square + turn right 90°
void yellow_move(struct DC_motor *mL, struct DC_motor *mR){
    reverse_square(mL,mR);
    TurnRight(mL,mR);
    TurnRight(mL,mR);
}


// Reverse one square + turn left 90°
void pink_move(struct DC_motor *mL, struct DC_motor *mR){
    reverse_square(mL,mR);
    TurnLeft(mL,mR);
    TurnLeft(mL,mR);
}


// Turn right 90° + Undo reverse one square
void reverse_yellow_move(struct DC_motor *mL, struct DC_motor *mR){
    TurnRight(mL,mR);
    TurnRight(mL,mR);
    forward_square(mL,mR);
    blue_move(mL,mR);
}


// Turn left 90° + Undo reverse one square
void reverse_pink_move(struct DC_motor *mL, struct DC_motor *mR){
    TurnLeft(mL,mR);
    TurnLeft(mL,mR);
    forward_square(mL,mR);
    blue_move(mL,mR);
}

// Execute reverse instructions
void white_move(struct DC_motor *mL, struct DC_motor *mR){
    
    blue_move(mL,mR); // First Turn back 180

    unsigned int temp_time; 
    card_func temp_func;
    
    //Loop through the two lists sequentially and move, starting with timer list
    while(time_index > 0){
        temp_time = get_timing(); // get the last movement time 
        
        //Move car forward for specified time and stop
        move_forward(mL,mR,temp_time); 
        stop(mL,mR);
        __delay_ms(1000); // Just some slack
        
        if (func_index > 0){
            //Execute function in function pointer 
            temp_func = get_function_ptr();
            temp_func(mL,mR);
            stop(mL,mR);
        }
        __delay_ms(1000); // Just some slack
    }
    stop(mL,mR);
    LightsToggle(); // Flash lights to signify end
}
