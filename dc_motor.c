#include <xc.h>
#include "dc_motor.h"

char peak_power = 25; // Toggle to increase speed 

// Initialise values to be calibrated
int turning_time = 116; // 45 degree turn time 
int reverse_time = 2600; // Reverse_square time

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


void setMotorPWM(struct DC_motor *m) {
	int PWMduty;

	if (m->direction){ // Forward - low time increases with power
		PWMduty = m->PWMperiod - ((int)(m->power)*(m->PWMperiod))/100; 
	}
	else {             // Reverse - high time increases with power
		PWMduty =((int)(m->power)*(m->PWMperiod))/100; 
	}
    
	*(m->dutyHighByte) = PWMduty; // Set high duty cycle byte 
        
    // Set dir_pin bit in LAT to high/low without changing other bits
	if (m->direction){
		*(m->dir_LAT) = *(m->dir_LAT) | (1<<(m->dir_pin));
	} else {
		*(m->dir_LAT) = *(m->dir_LAT) & (~(1<<(m->dir_pin)));
	}
}


void stop(struct DC_motor *mL, struct DC_motor *mR){
    
    while ((mL->power + mR->power) > 0){ //Decrement values gradually
        if(mL->power > 0){
            mL->power = mL->power -5;  
        }
        if(mR->power > 0){
            mR->power = mR->power - 5;
        }
        setMotorPWM(mL);
        setMotorPWM(mR);
    }
    // Ensure powers are definitively zero 
    mL->power = 0;
    mR->power = 0;
    setMotorPWM(mL);
    setMotorPWM(mR);
}


void move_forward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration){
    stop(mL,mR); 
    mL->direction = 1;
    mR->direction = 1;
    setMotorPWM(mR);
    setMotorPWM(mL);
    while ((mL->power + mR->power) < (2*peak_power)){ // Increment values gradually up to 25 each
        mR->power = mR->power + 5;
        mL->power = mL->power + 5 ;
        setMotorPWM(mL);
        setMotorPWM(mR);
    }
    custom_delay_ms(duration);
}


void move_backward(struct DC_motor *mL, struct DC_motor *mR, unsigned int duration){
    stop(mL,mR); // Ensure motor is idle 
    
    mL->direction = 0;
    mR->direction = 0;
    setMotorPWM(mR);
    setMotorPWM(mL);
    
    while ((mL->power + mR->power) < (80 + 2* peak_power) ){ // Increment values gradually up to 40+25 each
        mL->power = mL->power + 5;   
        mR->power = mR->power + 5;
        setMotorPWM(mR);
        setMotorPWM(mL);
    }
    
    // Add some extra power to make it move instep with forward function
    mL->power = mL->power + 2;   
    mR->power = mR->power + 2;
    setMotorPWM(mR);
    setMotorPWM(mL);
    
    custom_delay_ms(duration);
}


void reverse_square(struct DC_motor *mL, struct DC_motor *mR){
    move_backward(mL,mR,reverse_time);
    stop(mL,mR);
    __delay_ms(250); // Time to stabilise 
}


void forward_square(struct DC_motor *mL, struct DC_motor *mR){
    move_forward(mL,mR,reverse_time); 
    stop(mL,mR);
    __delay_ms(250); // Time to stabilise 
}



void TurnLeft(struct DC_motor *mL, struct DC_motor *mR){ 
    stop(mL,mR);       // Make sure motor is idle
    mL->direction = 0; 
    mR->direction = 1;
    setMotorPWM(mL);
    setMotorPWM(mR);
    
    while (mR->power != 80){
        mR->power = mR->power + 10;
        mL->power = mL->power + 10;
        setMotorPWM(mR); 
        setMotorPWM(mL);
    }    
    custom_delay_ms(turning_time);// Leave enough time to turn
    stop(mL,mR); 
    __delay_ms(150); // Wait for Car to stabilise
}


void TurnRight(struct DC_motor *mL, struct DC_motor *mR){
    stop(mL,mR);       // Make sure motor is idle
    mL->direction = 1; 
    mR->direction = 0;
    setMotorPWM(mL);
    setMotorPWM(mR);

    while (mL->power  != 80){
        mR->power  = mR->power + 10; 
        mL->power  = mL->power + 10; 
        setMotorPWM(mL);
        setMotorPWM(mR);
    }    
    custom_delay_ms(turning_time);// Leave enough time to turn
    stop(mL,mR); 
    __delay_ms(150); // Wait for Car to stabilise
}


void PrepareForTurn(struct DC_motor *mL, struct DC_motor *mR){
    move_backward(mL,mR,reverse_time/5);
    stop(mL,mR);
    __delay_ms(200); //Wait for buggy the stabilise
}

// See Header file for detailed instructions
void CalibrateTurns(struct DC_motor *mL, struct DC_motor *mR){
    
    LightsToggle(); 
    
    while(!(ButtonRF3 && ButtonRF2)){ // Press both buttons to exit calibration
        // Try turning 180 degrees in 4 motions
        PrepareForTurn(mL,mR);
        for(char k = 0; k<4; k++){
            TurnLeft(mL,mR);
        }
        
        // Note: Hold on pressing your desired buttons until LED flashes on
        while(!(ButtonRF2 || ButtonRF3)) // Wait for either to be pressed
        __delay_ms(1000);
            if(ButtonRF2 && ButtonRF3){ 
                LED1 = LED2 = 1;
            } else if(ButtonRF2){ 
                turning_time  -= 10; 
                LED1 = 1;
            } else if(ButtonRF3){
                turning_time  += 10; 
                LED2 = 1;
            }
        __delay_ms(2000); // Leave time to exit Calibration   
        LED1 = 0;
        LED2 = 0;
    }
    LightsToggle(); 
}


// See Header file for detailed instructions
void CalibrateReverseSquare(struct DC_motor *mL, struct DC_motor *mR){
    
    LightsToggle();
    
    while(!(ButtonRF3 && ButtonRF2)) { // Press both buttons to exit calibration
        reverse_square(mL,mR);  // Perform reverse_square movement
        stop(mL,mR);
        
        while(!(ButtonRF3 || ButtonRF2)) // Wait for either to be pressed
        __delay_ms(1000);
        if(ButtonRF2 && ButtonRF3){
            LED1 = LED2 = 1;
        } else if(ButtonRF2){ 
            reverse_time  -=  150;
            LED1 = 1;
        } else if(ButtonRF3){ 
            reverse_time  +=  150; 
            LED2 = 1;
        }
        __delay_ms(2000); // Leave time to exit Calibration
        LED1 = 0;
        LED2 = 0;
    }
    LightsToggle(); 

}