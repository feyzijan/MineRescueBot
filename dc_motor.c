#include <xc.h>
#include "dc_motor.h"

// function initialise T2 and PWM for DC motor control
void initDCmotorsPWM(int PWMperiod){
	//initialise your TRIS and LAT registers for PWM
    LATEbits.LATE2=0;   // initial  state
    LATEbits.LATE4=0;   // initial  state
    LATCbits.LATC7=0;   // initial  state
    LATGbits.LATG6=0;   // initial  state
  
    TRISEbits.TRISE2=0; // output
    TRISEbits.TRISE4=0; // output
    TRISCbits.TRISC7=0; // output
    TRISGbits.TRISG6=0; // output

    // timer 2 config
    T2CONbits.CKPS=011; // 1:8 prescaler (for 10 kHz required was 6.72 but closest was 8)
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=PWMperiod; //Period reg 10kHz base period
    T2CONbits.ON=1;

    RE2PPS=0x0A; //PWM6 on RE2
    RC7PPS=0x0B; //PMW7 on RC7

    PWM6DCH=0; //0% power
    PWM7DCH=0; //0% power

    PWM6CONbits.EN = 1;
    PWM7CONbits.EN = 1;
}


// function to set PWM output from the values in the motor structure
void setMotorPWM(struct DC_motor *m)
{
	int PWMduty; //tmp variable to store PWM duty cycle

	if (m->direction){ //if forward
		// low time increases with power
		PWMduty=m->PWMperiod - ((int)(m->power)*(m->PWMperiod))/100;
	}
	else { //if reverse
		// high time increases with power
		PWMduty=((int)(m->power)*(m->PWMperiod))/100;
	}

	*(m->dutyHighByte) = PWMduty; //set high duty cycle byte

	if (m->direction){ // if direction is high
		*(m->dir_LAT) = *(m->dir_LAT) | (1<<(m->dir_pin)); // set dir_pin bit in LAT to high without changing other bits
	} else {
		*(m->dir_LAT) = *(m->dir_LAT) & (~(1<<(m->dir_pin))); // set dir_pin bit in LAT to low without changing other bits
	}
}

//function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
    while (mL->power + mR->power > 0)
    {
        if(mL->power > 0){
            mL->power-=1;    
        }
        if(mR->power > 0){
            mR->power-=1;
        }       
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1); // delay for gradual change      
    }
} 

//function to make the robot turn right 
void turnRight(struct DC_motor *mL, struct DC_motor *mR)
{
    char new_power_R = (mR->power) * 5/10;
    char new_power_L = (mL->power) * 11/10;
    if (mL->power == 0 && mR->power == 0)
    {
        while ((mL->power + mR->power) != 50){
            if(mL->power <= 50){
                mL->power  ++;  
            }
            if(mR->power > 0){
                mR->power --;
            }
            setMotorPWM(mL);
            setMotorPWM(mR);
            __delay_ms(1); 
    }
    }
    else{
        while (mR->power > new_power_R || mL->power < new_power_L) {
            if (mR->power > new_power_R){mR->power  = mR->power -1;} // slow down left wheels
            if (mL->power < new_power_L){mL->power  = mL->power +1;} // speed up right wheels
            setMotorPWM(mR); //update
            setMotorPWM(mL); //update
            __delay_ms(1);
    }
    }
}

//function to make the robot turn right 
void turnLeft(struct DC_motor *mL, struct DC_motor *mR)
{
    char new_power_L = (mL->power) * 5/10;
    char new_power_R = (mR->power) * 11/10;
    if (mR->power == 0 && mL->power == 0)
    {
        while ((mR->power + mL->power) != 50){
            if(mR->power <= 50){
                mR->power  ++;  
            }
            if(mL->power > 0){
                mL->power --;
            }
            setMotorPWM(mR);
            setMotorPWM(mL);
            __delay_ms(1); 
    }
    }
    else{
        while (mL->power > new_power_L || mR->power < new_power_R) {
            if (mL->power > new_power_L){mL->power  = mL->power -1;} // slow down left wheels
            if (mR->power < new_power_R){mR->power  = mR->power +1;} // speed up right wheels
            setMotorPWM(mL); //update
            setMotorPWM(mR); //update
            __delay_ms(1);
    }
    }
}

//function to make the robot go straight
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR)
{
    mL->direction = 1;
    mR->direction = 1;
    
    // Increment values gradually 
    // note that left and right power are equal 
    while ((mL->power + mR->power < 100)){

        if(mL->power < 50){
            mL->power +=1;           
        }          
        if(mR->power < 50){
            mR->power +=1;
        } 
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1); 
    }
}
