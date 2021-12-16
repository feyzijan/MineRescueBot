#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>
#include "timers.h"
#include "LEDsButtons.h"

#define _XTAL_FREQ 64000000


/*************************  Variable Prototypes ********************************/

char peak_power; 
/* Toggle this variable in dc_motor.c to modify vehicle speed
 * Note that this only effects forward and reverse operation speed, not turning
 * Don't exceed 60, as back wheels get 40 + peak_power power to operate properly
 * !!Don't forget to update/calibrate reverse_time appropriately!!
 */

int turning_time; // Time(ms) needed for a 45 degree turn 

int reverse_time; // Time(ms) needed to reverse one square

/* Suggested reverse_time and power combinations (CAGB Floor)
 * Power:25 Reverse_time: 2600 -- Power:30, Reverse_time:1880 
 */

// Definition of DC_motor structure
struct DC_motor { 
    char power;                  // Motor power, out of 100
    char direction;              // Motor direction, forward(1), reverse(0)
    unsigned char *dutyHighByte; // PWM duty high byte address
    unsigned char *dir_LAT;      // LAT address for direction pin
    char dir_pin;           // Pin# controlling direction on LAT (L:RE4, R:RG6)
    int PWMperiod;               // Base period of PWM cycle
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
 * Decrements the power of left and right motors gradually, by 5 at a time
 *************/ 
void stop(struct DC_motor *mL, struct DC_motor *mR);


/*************
 * Function to make the robot go forward
 * First ensures that motors are stopped and direction is set to 1 
 * Gradually increases the power on each motor to peak_power
 * Takes optional time input (ms) for timed movements (forward_square)
 * Keeps moving forward until stopped with stop function
 * *************/ 
void move_forward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration);


/*************
 * Function to make the robot go forward
 * First ensures that motors are stopped and direction is set to 0 
 * Gradually increases the power on each motor to peak_power + 42(starting level)
 * Takes optional time input (ms) for timed movements  (reverse_square)
 * Keeps moving backward until stopped with stop function
 * *************/ 
void move_backward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration);


/*************
 * Function to make the robot perform "reverse one square" and then stop
 * Calls the move_backward method with the global variable reverse_time as an input
 *************/ 
void reverse_square(struct DC_motor *mL, struct DC_motor *mR);


/************* 
 * Function that makes the robot go forward one square and then stop
 * Same as reverse_square but calls move_forward inside the function
 * Used while the buggy is going back home
 *************/ 
void forward_square(struct DC_motor *mL, struct DC_motor *mR);



/*************
 * Function to make buggy do a 45° left turn 
 * Stops motor, sets left motor to reverse and right motor to forward direction
 * gradually increases power on both wheel, then stops 
 * Uses global variable turning_time to time delays 
 **************/ 
void TurnLeft(struct DC_motor *mL, struct DC_motor *mR);


/*************
 * Function to make buggy do a 45° right turn
 * Stops motor, sets right motor to reverse and left motor to forward direction
 * Stops motor - gradually increases power on both wheel, then stops 
 * Uses global variable turning_time to time delays
 **************/ 
void TurnRight(struct DC_motor *mL, struct DC_motor *mR);


/*************
 * Function to make buggy move backwards before executing a turn
 * Calls the move_backward method with the global variable reverse_time/5 as an input
 * This is needed because the car may have hit the wall as it detect colour
 **************/ 
void PrepareForTurn(struct DC_motor *mL, struct DC_motor *mR);


/*************
 * Function to calibrate turning to ensure accurate turns in new surface
 * Lights flash to indicate start of calibration
 * The buggy then performs a 180 turn - User has 4 options
 * Car turns too much -> Hold Button RF2: Decrease turning_time
 * Car turns too little -> Hold Button RF3: Increase turning_time
 * Want to see again? -> Hold Button RF2 and RF3 simultaneously
 * Buttons must be held till respective LED flashes
 * Hold Button RF2 and RF3 until back lights flash to exit calibration
 * Lights flash to indicate end of calibration
 **************/ 
void CalibrateTurns(struct DC_motor *mL, struct DC_motor *mR);


/*************
 * Function to calibrate reverse_time to ensure accurate distance
 * Lights flash to indicate start of calibration
 * The buggy then performs a reverse_square move - User has 4 options
 * Car moves too much -> Hold Button RF2: Decrease turning_time
 * Car moves too little -> Hold Button RF3: Increase turning_time
 * Want to see again? -> Hold Button RF2 and RF3 simultaneously
 * Buttons must be held till respective LED flashes
 * Hold Button RF2 and RF3 until back lights flash to exit calibration
 * Lights flash to indicate start of calibration
 **************/ 
void CalibrateReverseSquare(struct DC_motor *mL, struct DC_motor *mR);
#endif
