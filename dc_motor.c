#include <xc.h>
#include "dc_motor.h"


void initDCmotorsPWM(){
	//Initialise your TRIS and LAT registers for PWM

    // timer 2 config
    T2CONbits.CKPS=011; // 1:8 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=199; //199-Period reg 10kHz base period
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
            mL->power = mL->power -1;  
        }
        if(mR->power > 0){
            mR->power = mR->power - 1;
        }
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(100);  // delay for gradual change
    }    
}


void move_forward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration)
{
    stop(mL,mR); // start from rest
    
    // Update directions
    mL->direction = 1;
    mR->direction = 1;
      
    // Increment values gradually 
    while ((mL->power + mR->power) < 50){
        
        if(mL->power < 25){
            mL->power ++;
        }          
        if(mR->power < 25){
            mR->power ++;
        } 
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(100); 
    }
    custom_delay_ms(duration);
}


void move_backward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration)
{
    stop(mL,mR); // start from rest
    
    // Reverse Direction
    mL->direction = 0;
    mR->direction = 0;
      
    // Increment values gradually 
    while ((mL->power + mR->power) < 50){
        
        if(mL->power < 25){
            mL->power ++;
        }          
        if(mR->power < 25){
            mR->power ++;
        } 
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(100); 
    }
    custom_delay_ms(duration);
}


void TurnLeft(struct DC_motor *mL, struct DC_motor *mR)
{
    stop(mL,mR); // ensure it is stationary
    //set left motors to zero and right motors to some level 
    while (mR->power != 25){
            mR->power ++;
        setMotorPWM(mR);
        __delay_us(100); 
    }    
    
    friction_delay_ms();//leave enough time to turn
    stop(mL,mR); //stop turning
    
}


void TurnRight(struct DC_motor *mL, struct DC_motor *mR)
{
    stop(mL,mR); // ensure it is stationary
    
    //set right motors to zero and right motors to full power 
    while (mL->power  != 25){
        mL->power  ++;  
        setMotorPWM(mL);
        __delay_us(100); 
    }    
    
    friction_delay_ms();//leave enough time to turn
    stop(mL,mR); //stop turning
}

