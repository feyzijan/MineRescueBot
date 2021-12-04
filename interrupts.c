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
    
    PIE0bits.INT0IE = 1; //Enable INT0IE interrupt
    PIR0bits.INT0IF = 0; // Turn off Interrupt Flag initially
    
    INTCONbits.INT0EDG = 1; // Interrupt on rising edge 
    IPR0bits.INT0IP = 1; // High Priority
    
    //Not sure if I need these parameters (tried with these on and off)
    WPUBbits.WPUB0 = 1; // Enable weak pull up resistor?
    ODCONBbits.ODCB0 = 1; // Open-drain enabled?
    //INLVLBbits.INLVLB0 = 1; //1 = ST, 0=TTL? - Tried both ways - doesn't work
 
    color_click_interrupt_init();
    
    INTCONbits.PEIE = 1; // Toggle peripheral interrupts
    INTCONbits.GIE = 1; //Toggle interrupts Globally
}

void color_click_interrupt_init(void){
    //__debug_break();
    color_writetoaddr(0x00, 0x13); //turn on Clicker Interrupt(write 1 to AIEN bit)
    //Configure interrupt thresholds RGBC clear channel: Low 256 High: 500
    color_writetoaddr(0x04, 0xFF); 
    color_writetoaddr(0x05, 0x00);  
    color_writetoaddr(0x06, 0x00); 
    color_writetoaddr(0x07, 0x02); 
    
    color_writetoaddr(0x0C, 0x01); // Persistence register = 1( 0x00 doesn't trigger as well)
}

/************************************
 * High priority interrupt service routine for:
 * TMR0 overflows(1s)
 * Serial transmission 
 * Clicker Interrupt
 ************************************/

void __interrupt(high_priority) HighISR()
{    
    //Colour Clicker RGBC Clear Channel Interrupt
    // NOTE: Flag is not raised when I manually set LATB0 to 1
    if(PIR0bits.INT0IF){
        //__debug_break();
        PIR0bits.INT0IF = 0;// Flag
        //I2C_2_Master_Write(0b11100110); // clear RGBC interrupt in Clicker - 
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








//Timer 0 Low priority interrupt //
/*
void __interrupt(low_priority) LOWISR(){ 
    sendCharSerial4('j');
    
    if(PIR0bits.TMR0IF == 1) { // check interrupt flag
            TMR0H = 0;    
            TMR0L = 0;  
            PIR0bits.TMR0IF = 0; // clear interrupt flag
       }  
}
 */

/*
 * Possible Interrupt Codes to Try*
 * Interrupt on change
 PIE0bits.IOCIE = 1; // Enable interrupt on change
 IOCBPbits.IOCBP0 = 1;// Interrupt on change positive edge enable   
 IOCBFbits.IOCBF0 = 0; // Clear flag     
 
 */