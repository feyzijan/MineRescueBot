#include <xc.h>
#include "timers.h"


int friction = 100;

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init()
{
    T0CON1bits.T0CS=0b100; // LFINTOSC = 31Khz
    T0CON1bits.T0ASYNC=1; 
    T0CON1bits.T0CKPS=0b0111 ; // Pre-scaler 1:128 - overflow in 1s

    T0CON0bits.T016BIT=0; //16bit mode	  
    TMR0L = 0b00001000;
    IPR0bits.TMR0IP = 1; //priority low
    T0CON0bits.T0EN=1;	//start the timer
}


// Use to time the movements

void Timer1_init(void){
    // Maximum 16.9 seconds
    TMR1CLKbits.CS = 0b0100; // Clock source: LFINTOSC(31KHz)
    T1CONbits.CKPS = 0b11; // select pre-scaler: 1:8 
    PIE5bits.TMR1IE = 1; //enables interrupt
    IPR5bits.TMR1IP = 1; //interrupt priority set high
    
    /* Initialise the timer registers at 0 */
    TMR1H = 0;
    TMR1L = 0;
    /*Enable timer and counter */
    T1CONbits.ON = 1; // Enable timer
    T1GCONbits.GE = 0; // Counter mode off
}

//returns TMR1 value in milisecond format
unsigned int getTMR1(void){
    unsigned int temp;
    temp = TMR1H<<8;
    temp += TMR1L;
    return temp *2580645/100000000;
    // each bit is 0.2580645 ms

}


void setTMR1(unsigned int t_start){
    TMR1H = t_start>>8;
    TMR1L = t_start & 0b11111111;
}


// ** Currently makes maximum 32678 * 10 ms delay
// Test if this is accurate enough
void friction_delay_ms(void){
    int i;
    for(i=0;i<friction;i++){
        __delay_ms(10);
    }
}

void custom_delay_ms(unsigned int delay_time){
    int i;
    for(i=0;i<delay_time;i++){
        __delay_ms(10);
    }
    
}
