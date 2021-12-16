#include <xc.h>
#include "timers.h"


void Timer0_init(void){
    T0CON1bits.T0CS = 0b010; // Closck Source = Fosc/4 = 16MHz
    T0CON1bits.T0ASYNC = 1; 
    T0CON1bits.T0CKPS = 0b1110 ; // Pre-scaler 1:16384: each bit = 1.024ms
    T0CON0bits.T016BIT = 1; // 16bit mode	  
    TMR0H = 0;
    TMR0L = 0;
    T0CON0bits.T0EN=1;	// Start timer
}


void getTMR0_in_ms(void){
    unsigned int temp = TMR0L; // Must read Low register first !
    temp = TMR0H<<8;
    add_timing(temp + temp / 42 ); // Approximate as each bit is 1.024ms
}


void ResetTMR0(void){
    TMR0H = 0; // Must set TMR0H first!
    TMR0L = 0;
}


void custom_delay_ms(int delay_time){
    for(unsigned int i=0;i<delay_time;i++){
        __delay_ms(1);
    }
}


