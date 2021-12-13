#include <xc.h>
#include "timers.h"
#include "dc_motor.h"
#include "CardMoves.h"


extern unsigned int timer0val;

//Note: Overflows in 134 seconds, 1 bit = 2.048ms
void Timer0_init()
{
    T0CON1bits.T0CS=0b010; // Fosc/4 = 16MHz
    T0CON1bits.T0ASYNC=1; 
    T0CON1bits.T0CKPS=0b1111 ; // Pre-scaler 1:32768

    T0CON0bits.T016BIT=1; //16bit mode	  
    TMR0H = 0;
    TMR0L = 0;
    T0CON0bits.T0EN=1;	//start the timer
}


void reset_Timer0(void){
    TMR0H = 0;
    TMR0L = 0;
}


void Timer1_init(void){
    // Will overflow every ~2.1 s
    TMR1CLKbits.CS = 0b0100; // Clock source: LFINTOSC(31KHz)
    T1CONbits.CKPS = 0b00; // select pre-scaler: 1 
    TMR1H = 0;
    TMR1L = 0;
    T1CONbits.ON = 1; // Enable timer
    T1GCONbits.GE = 0; // Counter mode off
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
    //unsigned int temp = t_start;
    
    //TMR0H = t_start>>8;
    //TMR0L = temp & 0b11111111;
}


void friction_delay_ms(void){
    int i;
    for(i=0;i<friction/10;i++){
        __delay_ms(10);
    }
}


void custom_delay_ms(unsigned int delay_time){
    unsigned int i;
    for(i=0;i<delay_time/10;i++){
        __delay_ms(10);
    }
}

