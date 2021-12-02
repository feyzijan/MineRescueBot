// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor.h"
#include "i2c.h"
#include "color.h"
#include "LEDs.h"
#include "serial.h"
#include "timers.h"
#include "interrupts.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

unsigned int color = 0;

void main(void){
    
    //Inits
    initUSART4();
    Interrupts_init();
    Timer0_init();
    color_click_init();
    
    //LEDs for Debugging
    TRISHbits.TRISH3 = 0; 
    LATHbits.LATH3 = 0;
    //LATHbits.LATH3 = !LATHbits.LATH3 ;
    
    
    while(1){

        if(timer_flag) { //1 second has passed 
            color = color_read_Red(); //read color

            TxBufferedString(" test "); // writes string to buffer
            sendTxBuf(); //interrupt will handle the rest
            
            //timer_flag =0;
            //__delay_ms(1000);
        } 
          
    }    
}

