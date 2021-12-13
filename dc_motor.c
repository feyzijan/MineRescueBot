#include <xc.h>
#include "dc_motor.h"


extern int friction;
extern int reverse_time;


void initDCmotorsPWM(int PWMperiod){
    // Timer 2 Configuration for PWM
    T2CONbits.CKPS=011; // 1:8 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=PWMperiod; //199-Period reg 10kHz base period
    T2CONbits.ON=1;
    // Direction Pins
    TRISEbits.TRISE4=0;
    TRISGbits.TRISG6=0;
    // Configure TRIS registers for output
    TRISEbits.TRISE2=0;
    TRISCbits.TRISC7=0;
    
    // Configure PWM pins
    RE2PPS=0x0A; //PWM6 on RE2
    RC7PPS=0x0B; //PMW7 on RC7
    // The pins start high so once it reaches value below it is set low
    PWM6DCH=0; //0% power
    PWM7DCH=0; //0% power
    //Enable PWM
    PWM6CONbits.EN = 1;
    PWM7CONbits.EN = 1;
}


void setMotorPWM(struct DC_motor *m)
{
	int PWMduty;

	if (m->direction){// forward - low time increases with power
		PWMduty = m->PWMperiod - ((int)(m->power)*(m->PWMperiod))/100; 
	}
	else {// reverse - high time increases with power
		PWMduty =((int)(m->power)*(m->PWMperiod))/100; 
	}

	*(m->dutyHighByte) = PWMduty; //set high duty cycle byte 
        
    
	if (m->direction){ // set dir_pin bit in LAT to high/low without changing other bits
		*(m->dir_LAT) = *(m->dir_LAT) | (1<<(m->dir_pin));
	} else {
		*(m->dir_LAT) = *(m->dir_LAT) & (~(1<<(m->dir_pin)));
	}
}


void stop(struct DC_motor *mL, struct DC_motor *mR)
{
    //decrement values gradually 
    while ((mL->power + mR->power) > 0){
        if(mL->power > 0){
            mL->power = mL->power -5;  
        }
        if(mR->power > 0){
            mR->power = mR->power - 5;
        }
        setMotorPWM(mL);
        setMotorPWM(mR);
    }
    // Ensure powers are zero
    mL->power = 0;
    mR->power = 0;
    setMotorPWM(mL);
    setMotorPWM(mR);
}


void move_backward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration)
{
    stop(mL,mR);  
    mL->direction = 0;
    mR->direction = 0;
   
    // Increment values gradually up to 40+30 each
    while ((mL->power + mR->power) < 140){
        mL->power = mL->power + 5;
        mR->power = mR->power + 5;
        setMotorPWM(mL);
        setMotorPWM(mR);
    }
    custom_delay_ms(duration);
}


void move_forward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration)
{
    stop(mL,mR); 
    mL->direction = 1;
    mR->direction = 1;
      
    // Increment values gradually up to 30 each
    while ((mL->power + mR->power) < 60){   
        mL->power = mL->power + 5 ;
        mR->power = mR->power + 5;
        setMotorPWM(mL);
        setMotorPWM(mR);
    }
    custom_delay_ms(duration);
}


void reverse_square(struct DC_motor *mL, struct DC_motor *mR){
    move_backward(mL,mR,reverse_time);
    stop(mL,mR);
}


void forward_square(struct DC_motor *mL, struct DC_motor *mR){
    move_forward(mL,mR,reverse_time);
    stop(mL,mR);
}



void TurnLeft(struct DC_motor *mL, struct DC_motor *mR)
{ 
    stop(mL,mR); // Make sure power is zero
    
    // Set directions
    mL->direction = 0; // make use of Left wheels for turn as well
    mR->direction = 1;
    setMotorPWM(mL);
    setMotorPWM(mR);
    
    while (mR->power != 80){
        mR->power = mR->power + 10;
        mL->power = mL->power + 10;
        setMotorPWM(mR);
        setMotorPWM(mL);
    }    
    friction_delay_ms();// Leave enough time to turn
    stop(mL,mR); 
}


void TurnRight(struct DC_motor *mL, struct DC_motor *mR)
{
    stop(mL,mR); 
    
    // Set directions
    mL->direction = 1;
    mR->direction = 0;
    setMotorPWM(mL);
    setMotorPWM(mR);

    
    while (mL->power  != 80){
        mL->power  = mL->power + 10; 
        mR->power  = mR->power + 10;  
        setMotorPWM(mL);
        setMotorPWM(mR);
        
    }    
    friction_delay_ms();// Leave enough time to turn
    stop(mL,mR); 
}


void PrepareForTurn(struct DC_motor *mL, struct DC_motor *mR){
    move_backward(mL,mR,reverse_time/15); // DECIDE ON FRACTION
    stop(mL,mR);
}


void CalibrateTurns(struct DC_motor *mL, struct DC_motor *mR){
    
    LightTest(); // Toggle Light to indicate start of Calibration
    
    // Store initial value so the 5% adding/subtracting don't compound
    int temp = friction; 

    while(!(ButtonRF3 && ButtonRF2)) // Press both buttons to exit calibration
    {
        PrepareForTurn(mL,mR);
        __delay_ms(100); //Wait for buggy the stabilize
        // Try turning 180 degrees in 4 motions
        for(int k = 0; k<4; k++){
            TurnLeft(mL,mR);
            __delay_ms(500); // Time lag to check each 45 degree turn
        }
        
        
        // Note: Hold on pressing your desired buttons until LED flashes on
        while(!(ButtonRF2 || ButtonRF3)) // Wait for either to be pressed
        __delay_ms(1000);
            if(ButtonRF2 && ButtonRF3){ // No change
                LED1 = 1;
                LED2 = 1;
            } else if(ButtonRF2){
                friction  -= temp /40; //2.5% Decrease
                LED1 = 1;
            } else if(ButtonRF3){
                friction  += temp /40;; //2.5% Increase
                LED2 = 1;
            }
        __delay_ms(1500); // Leave two second to exit Calibration   
        LED1 = 0;
        LED2 = 0;
    }
    LightTest(); // Indicate end of Calibration
}



void CalibrateReverseSquare(struct DC_motor *mL, struct DC_motor *mR){
    
    LightTest(); // Toggle Light to indicate start of Calibration
    
    // Store initial value so the 5% adding/subtracting don't compound
    int temp = reverse_time; 
    
    while(!(ButtonRF3 && ButtonRF2)) // Press both buttons to exit calibration
    {
        reverse_square(mL,mR);  // Perform reverse_square movement
        stop(mL,mR);
        
        while(!(ButtonRF3 || ButtonRF2)) // Wait for either to be pressed
            __delay_ms(1000);
            if(ButtonRF2 && ButtonRF3){ // No change
                LED1 = 1;
                LED2 = 1;
            } else if(ButtonRF2){
                reverse_time  -=  temp /40; //2.5% Decrease
                LED1 = 1;
            } else if(ButtonRF3){
                reverse_time  +=  temp /40; //2.5% Increase
                LED2 = 1;
            }
        __delay_ms(1500); // Have one second to exit Calibration
        LED1 = 0;
        LED2 = 0;
    }
    LightTest(); // Indicate end of Calibration
}

