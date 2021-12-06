#include <xc.h>
#include "interrupts.h"
#include "serial.h"
#include "i2c.h"
#include "color.h"
#include "LEDs.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
    INTCONbits.IPEN = 1; // Enable priority levels
    PIE0bits.TMR0IE = 1; //Enable Timer0 interrupt
    
    /***********************Clicker Interrupt Init****************************/
    //Pins for Clicker Interrupt
    TRISBbits.TRISB0 = 1; //RB0 as input
    ANSELBbits.ANSELB0 = 0; // RB0 as digital input
    
    // Configure INT0IE interrupt
    PIE0bits.INT0IE = 1; //Enable INT0IE interrupt
    PIR0bits.INT0IF = 0; // Turn off Interrupt Flag initially
    INTCONbits.INT0EDG = 0; // Interrupt on falling edge 
    IPR0bits.INT0IP = 1; // High Priority
    
    color_click_interrupt_init();
    //INTCONbits.PEIE = 1; // Toggle peripheral interrupts - appears unnecessary
    INTCONbits.GIE = 1; //Toggle interrupts Globally
}

/************************************
 * High priority interrupt service routine for:
 * TMR0 overflows(1s) (For Testing)
 * Serial transmission (For Testing)
 * Clicker Interrupt
 ************************************/
void __interrupt(high_priority) HighISR()
{    
    //Colour Clicker RGBC Clear Channel Interrupt
    // NOTE: Flag is not raised when I manually set LATB0 to 1
    if(PIR0bits.INT0IF){
        //__debug_break();
        color_int_clear();
        PIR0bits.INT0IF = 0;// Flag
        HeadLamp = !HeadLamp;
        
    }

    // Interrupt for transmitting data- For Testing only
    if(PIR4bits.TX4IF){
        sendCharSerial4(getCharFromTxBuf()); // read buffer and send
        if(!isDataInTxBuf()) {
            PIE4bits.TX4IE = 0; //disable interrupt if buffer is empty
        }
    }
    
    // Timer 0 Interrupt - Triggers every second
    if(PIR0bits.TMR0IF) {
            timer_flag = 1;
            TMR0L = 0b00001000;
            PIR0bits.TMR0IF = 0; // clear flag
        }
}
