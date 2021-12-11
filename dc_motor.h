#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>
#include "timers.h"
#include "LEDs.h"

#define _XTAL_FREQ 64000000


/* TODO:
 * Use pointers to update reverse time and friction values
 * Make TurnLeft/Right functions turn 45 degrees
 * Consider making stop function stop quicker
 */

// Estimated time of all functions is about 3ms

/*************************  Variable Prototypes ********************************/


int friction; // time (in ms) needed for a 45 degree turn (320 for Room-749 surface)
//Can be updated anywhere

int reverse_time; // Time(in ms) required for reverse one square
//Must be updated inside the main loop if it'll be used there

// Definition of DC_motor structure
// Note that I have defined 0 direction as forward due to with clicker's position
struct DC_motor { 
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(0), reverse(1)
    unsigned char *dutyHighByte; //PWM duty high byte address
    unsigned char *dir_LAT; //LAT address for direction pin
    char dir_pin; // pin number that controls direction on LAT (L:RE4, R:RG6)
    int PWMperiod; //base period of PWM cycle
};


/*************************  Function Prototypes ********************************/


/************
 * Function initialise T2 and PWM for DC motor control
 ************/
void initDCmotorsPWM(int PWMperiod);


/*************
 * Function to perform low level update on motor power and direction via PWM 
 *************/
void setMotorPWM(struct DC_motor *m);


/*************
 * Function to stop the buggy's movement
 * Decrements the power of left and right motors gradually
 *************/ 
void stop(struct DC_motor *mL, struct DC_motor *mR);


/*************
 * Function to make the robot go forward
 * Sets direction to 0, gradually increases the power on each motor
 * Takes optional time input for timed movements
 *************/ 
void move_forward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration);


/*************
 * Function to make the robot go backwards
 * Sets direction to 1, gradually increases the power on each motor
 * Takes optional time input for timed movements
 *************/ 
void move_backward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration);


/*************
 * Function to make the robot perform "reverse one square"
 * Calls the move_backward method with the global variable reverse_time as an input
 * Then stops
 * This is so that the reverse_time variable used can be updated globally
 *************/ 
void reverse_square(struct DC_motor *mL, struct DC_motor *mR);


/* Function that makes the robot go forward one square
 * Same as reverse_square but calls move_forward inside the function
 */
void forward_square(struct DC_motor *mL, struct DC_motor *mR);



/*************
 * Function to make buggy do a 45degree left turn 
 * Stops motor - gradually increases power on right wheels - then stops again
 * NOTE: Global friction variable must be set according to surface friction for accurate turns
 * Picking correct friction value requires testing and calibration  
 **************/ 
void TurnLeft(struct DC_motor *mL, struct DC_motor *mR);


/*************
 * Function to make buggy do a 45degree right turn
 * Stops motor - gradually increases power on left wheels - then stops again
 * NOTE: Global friction variable must be set according to surface friction for accurate turns
 * Picking correct friction value requires testing and calibration  
 **************/ 
void TurnRight(struct DC_motor *mL, struct DC_motor *mR);

#endif