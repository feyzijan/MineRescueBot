#include <xc.h>
#include "dc_motor.h"


extern int friction;
extern int reverse_time;

 

void initDCmotorsPWM(int PWMperiod){
	//Initialise your TRIS and LAT registers for PWM

    // timer 2 config
    T2CONbits.CKPS=011; // 1:8 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=PWMperiod; //199-Period reg 10kHz base period
    T2CONbits.ON=1;
    
    // Direction Pins
    TRISEbits.TRISE4=0;
    TRISGbits.TRISG6=0;
    
    // COnfigure TRIS registers for output
    TRISEbits.TRISE2=0;
    TRISCbits.TRISC7=0;
    
    RE2PPS=0x0A; //PWM6 on RE2
    RC7PPS=0x0B; //PMW7 on RC7
   
    //the pins start high so once it reaches value below it is set low
    PWM6DCH=0; //0% power
    PWM7DCH=0; //0% power
    
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
}


void move_backward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration)
{
    stop(mL,mR); 
    
    mL->direction = 1;
    mR->direction = 1;
      
    // Increment values gradually 
    while ((mL->power + mR->power) < 100){
        
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
    
    mL->direction = 0;
    mR->direction = 0;
      
    // Increment values gradually - NOTE For some reason Power is = power - 50 or smt
    while ((mL->power + mR->power) < 150){   
        mL->power = mL->power + 10 ;
        mR->power = mR->power + 10;
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
    stop(mL,mR); 
 
    while (mR->power != 80){
        mR->power = mR->power + 10;
        setMotorPWM(mR);
    }    
    friction_delay_ms();// Leave enough time to turn
    stop(mL,mR); 
}


void TurnRight(struct DC_motor *mL, struct DC_motor *mR)
{
    stop(mL,mR); 
    
    while (mL->power  != 80){
        mL->power  = mL->power + 10;  
        setMotorPWM(mL);
    }    
    friction_delay_ms();// Leave enough time to turn
    stop(mL,mR); 
}