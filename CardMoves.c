#include <xc.h>
#include "CardMoves.h"
#include "dc_motor.h"




void pick_move(char color, char *move_list, struct DC_motor *mL, struct DC_motor *mR){
    
    if (color == 'w'){
        //white_move(mL,mR, move_list); // pass move list so you can revert it 
    } else {
        *move_list = color; // add the color to the list of moves
        if(color == 'r'){
            red_move(mL,mR);
        } else if(color == 'g'){
            green_move(mL,mR);
        } else if(color == 'b'){
            blue_move(mL,mR);
        } else if (color == 'y'){
            yellow_move(mL,mR);
        } else if (color == 'o'){
            orange_move(mL,mR);
        } else if (color == 'l'){
            lightblue_move(mL,mR);
        }
    }
}


/********** Red Card: Turn Right  90° *************************/
void red_move(struct DC_motor *mL, struct DC_motor *mR) {
    fullTurnRight(mL,mR);
    
}

/********** Green Card: Turn Left 90° ************************/
void green_move(struct DC_motor *mL, struct DC_motor *mR){
    fullTurnLeft(mL,mR);
}

/********** Blue Card: Turn back 180°*********************/
void blue_move(struct DC_motor *mL, struct DC_motor *mR){
    fullTurnLeft(mL,mR);
    fullTurnLeft(mL,mR);
}

/********** Yellow Card: Reverse one square + turn right 90°***/
void yellow_move(struct DC_motor *mL, struct DC_motor *mR){

    fullTurnRight(mL,mR);
}

/********** Yellow Card: Reverse one square + turn left 90° ***/
void pink_move(struct DC_motor *mL, struct DC_motor *mR){
    //reverse
    fullTurnLeft(mL,mR);
}

/********** Orange Card: Turn Right 135° s***************/
void orange_move(struct DC_motor *mL, struct DC_motor *mR){

    fullTurnRight(mL,mR);

}


/********** LightBlue Card: Turn Left 135° ****************/
void lightblue_move(struct DC_motor *mL, struct DC_motor *mR){
    fullTurnLeft(mL, mR);

}


/********** White Card: Finish - execute reverse instructions ***************/
void white_move(char *move_list, struct DC_motor *mL, struct DC_motor *mR){
    // construct loop to loop back through 
    while ( *move_list != 0){ // Check what happens in first element
        pick_move(*move_list,move_list,mL, mR);
        move_list = move_list - 7;
    }
    
    
}


// [ r, b, o , g, ]
