#include <xc.h>
#include "interrupts.h"
#include "i2c.h"
#include "color.h"
#include "LEDsButtons.h"
#include "timers.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
    INTCONbits.IPEN = 1; // Interrupt Priority Levels: Enable
    //PIE0bits.TMR0IE = 1; //TMR0 Interrupt: Enable
    
    //****Clicker Interrupt Initialisations
    TRISBbits.TRISB0 = 1; // Pin RB0: Input(1)
    ANSELBbits.ANSELB0 = 0; // Pin RB0: Digital input(0)
    
    PIE0bits.INT0IE = 1; //Interrupt on Pin RB0: Enable
    PIR0bits.INT0IF = 0; //Interrupt Flag: Off
    INTCONbits.INT0EDG = 0; // Interrupt on Falling Edge
    IPR0bits.INT0IP = 1; // Interrupt Priority: High
    
    color_click_interrupt_init(); // Write interrupt configurations to clicker
    
    // Enable all interrupts
    INTCONbits.PEIE = 1; 
    INTCONbits.GIE = 1; 
}

/************************************
 * High priority interrupt service routine for:
 * Clicker Interrupt
 ************************************/
void __interrupt(high_priority) HighISR()
{    
    //Colour Clicker RGBC Clear Channel Interrupt
    if(PIR0bits.INT0IF){
        getTMR0_in_ms(); // Log movement duration in memory
        BrakeLight = 1; // Testing 
        color_flag = 1; // Color_flag to indicate color reading with LED - //Testing
        color_click_interrupt_off(); // Turn off clicker interrupt(also clears it)
        PIR0bits.INT0IF = 0; // Clear Interrupt Flag
    }
}

