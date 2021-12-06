#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/* TODO: Add comments to each function 
 * Remove turnLeft/Right functions and replace them with fullTurnLeft/Right
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

void initDCmotorsPWM(int PWMperiod); // function to setup PWM


void setMotorPWM(struct DC_motor *m);


void stop(struct DC_motor *mL, struct DC_motor *mR);


void turnLeft(struct DC_motor *mL);


void turnRight(struct DC_motor *mR);


void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR);

void fullTurnLeft(struct DC_motor *mL, struct DC_motor *mR);

void fullTurnRight(struct DC_motor *mL, struct DC_motor *mR);


#endif
