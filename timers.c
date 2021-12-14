#include <xc.h>
#include "timers.h"
#include "dc_motor.h"
#include "CardMoves.h"


extern unsigned int timer0val;

/****** Timer0 Functions *********/

void Timer0_init() 
{
    T0CON1bits.T0CS=0b010; // Closck Source = Fosc/4 = 16MHz
    T0CON1bits.T0ASYNC=1; 
    T0CON1bits.T0CKPS=0b1111 ; // Pre-scaler 1:32768

    T0CON0bits.T016BIT=1; //16bit mode	  
    TMR0H = 0;
    TMR0L = 0;
    T0CON0bits.T0EN=1;	//start the timer
}


void getTMR0_in_ms(void){
    unsigned int temp = TMR0L; // Must read Low first
    temp = TMR0H<<8;
    timer0val = temp * 2  + temp / 21  ; // each bit is 2.048ms
    add_timing(timer0val);
}



void ResetTMR0(void){
    TMR0H = 0; // Must set TMR0H first
    TMR0L = 0;
}

void custom_delay_ms(unsigned int delay_time){
    unsigned int i;
    for(i=0;i<delay_time/2;i++){
        __delay_ms(2);
    }
}


