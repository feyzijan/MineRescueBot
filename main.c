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

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void main(void){
    
    //Initialisations
    initUSART4();
    color_click_init();
    LEDsInit();
    Interrupts_init();
    Timer0_init();
    initDCmotorsPWM(199); 

    char color;
    
    // Motor Initialisation
    //Initialise Motor structs
    struct DC_motor motorL, motorR; 
    motorL.power=0; 				
    motorL.direction=1; 
    motorL.dutyHighByte=(unsigned char *)(&PWM6DCH); 
    motorL.dir_LAT=(unsigned char *)(&LATE); 	
    motorL.dir_pin=4; 	/
    motorL.PWMperiod=199; 
    
    motorR.power=0; 						
    motorR.direction=1; 					
    motorR.dutyHighByte=(unsigned char *)(&PWM7DCH);	
    motorR.dir_LAT=(unsigned char *)(&LATG); 		
    motorR.dir_pin=6;
    motorR.PWMperiod=199; 		
    
    
    //LightToggle(); // Test LED operations
    
    // Button RF2 - for debugging
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin  
   
    
    while(1){
        /*****Test Motor Operation***//*
        stop(&motorL, &motorR);
        fullSpeedAhead(&motorL, &motorR);
        __delay_ms(1000);
        stop(&motorL, &motorR);
        */  
        
        /******** Send Back Colour Readings to PC ******/
        if(timer_flag) { 
            get_int_status(); //Send back Interrupt Status
            read_All_Colors();
            SendColorReadings(); 
            timer_flag =0;   
        } 
    }      
}