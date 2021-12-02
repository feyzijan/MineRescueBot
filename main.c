// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "i2c.h"
#include "color.h"
#include "dc_motor.h"
#include "serial.h"
#include "timers.h"
#include "interrupts.h"
#include <stdio.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void main(void){
    initUSART4();
    Interrupts_init();
    Timer0_init();
    
    color_click_init();
    
    unsigned int colorRed;
    unsigned int colorGreen;
    unsigned int colorBlue;
    
    char buf1[] = {0x00};
    char buf2[] = {0x00};
    char buf3[] = {0x00};
    // clicker board LEDs
    /*
    LATDbits.LATD7 = 1;
    LATHbits.LATH3 = 1;
    TRISDbits.TRISD7 = 0;
    TRISHbits.TRISH3 = 0; 
    
    //Head Lamps - Working
    TRISHbits.TRISH1 = 0; 
    LATHbits.LATH1 = 1;
    // Main Beam - Working
    TRISDbits.TRISD3 = 0; 
    LATDbits.LATD3 = 1;
    // Brake Lights - Working
    TRISDbits.TRISD4 = 0; 
    LATDbits.LATD4 = 1;
    ANSELDbits.ANSELD4=0;
    // Right Lamps - Working
    TRISHbits.TRISH0 = 0; 
    LATHbits.LATH0= 1;
    // Left Lamps - Working
    TRISFbits.TRISF0 = 0; 
    LATFbits.LATF0 = 1;
    ANSELFbits.ANSELF0=0;
    // BATV-Sense - Not Tested
    //TRISFbits.TRISF6 = 1;
    
    */
    
    // COLOR CLICK RGB LEDs
    
    // Red Led (G0 for MB1 G1 for MB2)
    TRISGbits.TRISG0 = 0; 
    LATGbits.LATG0 = 0;
    ANSELGbits.ANSELG0=0;
    
    // Blue Led (A3 for MB1 F7 for MB2)
    
    TRISAbits.TRISA3 = 0; 
    LATAbits.LATA3 = 0;
    ANSELAbits.ANSELA3=0;
    
    
    // Green Led (A3 for MB1 F6 for MB2)
    TRISEbits.TRISE7 = 0; 
    LATEbits.LATE7 = 0;
    ANSELEbits.ANSELE7=0;
    

    while(1){
        
        //Edit for Color
        
        if(timer_flag) { //1 second has passed 
            colorRed = color_read_Red(); //read red color
            colorGreen = color_read_Green(); //read green color
            colorBlue = color_read_Blue(); //read blue color

            sprintf(buf1,"%d",colorRed);
            
            
            
            //__debug_break();
            TxBufferedString("       RED: "); // writes string to buffer
            TxBufferedString(buf1); 
            sprintf(buf2,"%d",colorGreen);
            TxBufferedString("       GREEN: "); // writes string to buffer
            TxBufferedString(buf2); 
            sprintf(buf3,"%d",colorBlue);
            TxBufferedString("       BLUE: "); // writes string to buffer
            TxBufferedString(buf3); 
            sendTxBuf(); //interrupt will handle the rest
            
            timer_flag =0;
            //__delay_ms(1000);
        }
    }    
}
