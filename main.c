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
#include "CardMoves.h"
#include <stdio.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void main(void){
    initUSART4();
    color_click_init();
    LEDsInit();
    Interrupts_init();
    Timer1_init();    
    //initDCmotorsPWM(); 

    char color;
    // Set correct friction value for turns
    friction = 200; // TEST THAT THIS WORKS
    reverse_time = 15;
    
    unsigned int colorRed;
    unsigned int colorGreen;
    unsigned int colorBlue;
    unsigned int colorClear;
    
    char buf1[] = {0x00};
    char buf2[] = {0x00};
    char buf3[] = {0x00};
    char buf4[] = {0x00};
    
    // COLOR CLICK RGB LEDs
    
    LightToggle();
    
    // Button RF2 to toggle light on and off for testing
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin
    

    while(1){
        
        //Edit for Color
        /*
        if(timer_flag) { //1 second has passed 
            read_All_Colors(); // read colours

            sprintf(buf1,"%d",red);
            TxBufferedString(" ");
            TxBufferedString("\n");
            //TxBufferedString("RED:   "); // writes string to buffer
            TxBufferedString(buf1);
            TxBufferedString("\n");
            
            sprintf(buf2,"%d",green);
            //TxBufferedString("GREEN: "); // writes string to buffer
            TxBufferedString(buf2);
            TxBufferedString("\n");
            
            sprintf(buf3,"%d",blue);
            //TxBufferedString("BLUE:  "); // writes string to buffer
            TxBufferedString(buf3);
            TxBufferedString("\n");
            
            sprintf(buf4,"%d",clear);
            //TxBufferedString("CLEAR: "); // writes string to buffer
            TxBufferedString(buf4);
            TxBufferedString("\n");
            sendTxBuf(); //interrupt will handle the rest
            
            timer_flag =0;
         }
        */
        
  
        if (!PORTFbits.RF2) {
            LightToggle();
        }
            
        if (test_flag){
            //TxBufferedString("hello");
            //TxBufferedString("\n");
            LED1 = 1;
            color = decide_color();
            if (color == 4){
                LED1 = 0;
            }
            LightToggle();
            test_flag=0;
            //color_click_interrupt_init();
        }
    }
}

