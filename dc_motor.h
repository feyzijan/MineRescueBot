#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>
#include "timers.h"

#define _XTAL_FREQ 64000000


/* TODO:
 * Test all motor functions with different values of delay time between updates and power
 * Make TurnLeft/Right functions turn 45 degrees
 */

// Estimated time of all functions is about 3ms

/*************************  Variable Prototypes ********************************/


int friction;
// Time required for reverse one square
int reverse_time;


// Definition of DC_motor structure
struct DC_motor { 
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    unsigned char *dutyHighByte; //PWM duty high byte address
    unsigned char *dir_LAT; //LAT address for direction pin
    char dir_pin; // pin number that controls direction on LAT (L:RE4, R:RG6)
    int PWMperiod; //base period of PWM cycle
};


/*************************  Function Prototypes ********************************/


/************
 * Function initialise T2 and PWM for DC motor control
 ************/
void initDCmotorsPWM();


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
 * Sets direction to 1, gradually increases the power on each motor to 50
 * Note: Follow this with a delay and stop function to achieve predetermined distance
 * Execute delay equal to the duration in ms
 *************/ 
void move_forward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration);


/*************
 * Function to make the robot go forward
 * Sets direction to 0, gradually increases the power on each motor to 50
 * Note: Follow this with a delay and stop function to achieve predetermined distance
 * Execute delay equal to the duration in ms
 **************/ 
void move_backward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration);


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
