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

char checker;

void main(void){
    
    //Inits
    initUSART4(); 
    color_click_init();
    Interrupts_init();
    Timer0_init();
    
    LEDsInit();
    //LED1 = !LED1;
    //LightToggle();
    
    checker = PORTBbits.RB0;
    while(1){

        // Once every second
        if(timer_flag) { 
            
            read_All_Colors();
            SendColorReadings();
           
            timer_flag =0;   
        } 
    }    
}

