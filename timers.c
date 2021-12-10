#include <xc.h>
#include "timers.h"
#include "dc_motor.h"


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


void Timer1_init(void){
    // Will overflow every ~2.1 s
    TMR1CLKbits.CS = 0b0100; // Clock source: LFINTOSC(31KHz)
    T1CONbits.CKPS = 0b00; // select pre-scaler: 1 
    TMR1H = 0;
    TMR1L = 0;
    T1CONbits.ON = 1; // Enable timer
    T1GCONbits.GE = 0; // Counter mode off
}


//returns TMR1 value in milisecond format
unsigned int getTMR0_in_ms(void){
    unsigned int temp;
    temp = TMR10<<8;
    temp += TMR0L;
    temp = temp *2048/1000; 
    return temp;
    // each bit is 2.048ms
}


void setTMR0(unsigned int t_start){
    unsigned int temp = t_start;
    TMR0H = t_start>>8;
    TMR0L = temp & 0b11111111;
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