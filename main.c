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
#include "buttons.h"
#include <stdio.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void main(void){
    initUSART4();
    color_click_init();
    LEDsInit();
    Timer1_init();    
    //initDCmotorsPWM(199);
    buttonsInit();
    
    // COLOR CLICK RGB LEDs    
    LightToggle();

    // Calibrate interrupt threshold instructions:
    // 1. Hold blue card up to front of buggy with a few millimeters gap while perpendicular to the floor
    // 2. press the left button (RF3)
    // 3. Leave blue card in front of buggy for at least a second
    // 4. Remove blue card and press button and wait for at least one second
    // 5. Add black card approximately 7 cm away from the front of the buggy and press the button again, wait for one second
    // 6. Remove the black card
    // 7. Place buggy at the start point of the tracking course and press left button again to end calibration
    interrupt_threshold_calibrate();
    
    // Initialize interrupt after threshold calibration
    Interrupts_init();
    

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

    while(1){
        
        //Edit for Color
        
        if(timer_flag) { //1 second has passed 
            read_All_Colors(); // read colous
            /*
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
            */
            
            sprintf(buf2,"%d",int_high);
            //TxBufferedString("GREEN: "); // writes string to buffer
            TxBufferedString(buf2);
            TxBufferedString("\n");
            
            sprintf(buf3,"%d",int_low);
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
        
        
  
        if (!RightButton) {LightToggle();}                                  // turn RGB light off manually if required
        if (!LeftButton) {color_click_interrupt_init();BrakeLight=0;}       // turn on interrupt source manually for testing
            
        if (test_flag){
            LED1 = 0;
            color = decide_color();
            for (int i=0;i<color;i++){
                LED1 = 1;
                __delay_ms(350);
                LED1 = 0;
                __delay_ms(350);
            }
            LightToggle();
            test_flag=0;
        }
    }
}

