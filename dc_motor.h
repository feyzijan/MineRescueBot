#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>
#include "timers.h"

#define _XTAL_FREQ 64000000


/* TODO:
 *
 * 
 */


// Definition of DC_motor structure
struct DC_motor { 
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    unsigned char *dutyHighByte; //PWM duty high byte address
    unsigned char *dir_LAT; //LAT address for direction pin
    char dir_pin; // pin number that controls direction on LAT (L:RE4, R:RG6)
    int PWMperiod; //base period of PWM cycle
};


/********************** Function prototypes ********************************/

/*********
 * Function to set up the PWM module for the DC motor
 *********/
void initDCmotorsPWM(int PWMperiod);

/*********
 * Function to update motor parameters to execute desired motions 
 *************/
void setMotorPWM(struct DC_motor *m);



/******
 * Function to 
 ******/
void stop(struct DC_motor *mL, struct DC_motor *mR);


/*****************
 * Function to make the robot go full speed ahead ahead 
 ****************/ 
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR);


/********************
 * Function to make a stationary robot do a full left turn quickly  
 * Reduce power on left-side wheels to 0
 * Increase power on right-side wheels to 100
 ************************/ 
void TurnLeft(struct DC_motor *mL, struct DC_motor *mR);


/********************
 *Function to make a stationary robot do a full right turn quickly  
 * Reduce power on right-side wheels to 0
 * Increase power on left-side wheels to 100 
 *********************/ 
void TurnRight(struct DC_motor *mL, struct DC_motor *mR);


#endif
