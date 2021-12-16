#include <xc.h>
#include "interrupts.h"

void Interrupts_init(void)
{
    INTCONbits.IPEN = 1;          // Interrupt Priority Levels: Enable
   
    PIE0bits.TMR0IE = 1;          // Enable TMR0 interrupt
    IPR0bits.TMR0IP = 1;          // Set TMR0 IP: To high
    
    // Clicker Interrupt Initialisations
    TRISBbits.TRISB0 = 1;         // Pin RB0: Input(1)
    ANSELBbits.ANSELB0 = 0;       // Pin RB0: Digital input(0)
    PIE0bits.INT0IE = 1;          // Interrupt on Pin RB0: Enable
    PIR0bits.INT0IF = 0;          // Interrupt Flag: Off
    INTCONbits.INT0EDG = 0;       // Interrupt on Falling Edge
    IPR0bits.INT0IP = 1;          // Interrupt Priority: High
    color_click_interrupt_init(); // Write interrupt configurations to clicker
    
    // Enable all interrupts
    INTCONbits.PEIE = 1; 
    INTCONbits.GIE = 1; 
}


void __interrupt(high_priority) HighISR() {    
    //Clicker Interrupt
    if(PIR0bits.INT0IF){
        getTMR0_in_ms();              // Log forward movement duration in memory
        BrakeLight = 1;               // Indicate Interrupt has triggered
        color_flag = 1;               // Checked in main to read colour
        color_click_interrupt_off();  // Turn off (and clear) clicker interrupt
        PIR0bits.INT0IF = 0;          // Clear interrupt Flag
    }
    
    // Timer0 interrupt
    if(PIR0bits.TMR0IF == 1) { 
        lost_flag = 1;        // Flag set so main.c can return home
        PIR0bits.TMR0IF = 0;  // Clear interrupt flag
        PIE0bits.TMR0IE = 0;  // Disable TMR0 interrupt
    }
}

