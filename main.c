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


// Set correct friction value for turns
friction = 200; // TEST THAT THIS WORKS
reverse_time = 15;

void main(void){
    
    //friction = 300;
    
    //Initialisations
    initUSART4();
    color_click_init();
    LEDsInit();
    Interrupts_init();
    Timer1_init();
    
    initDCmotorsPWM(); 

    char color;
    
    // Motor Initialisation
    //Initialise Motor structs
    struct DC_motor motorL, motorR; 
    motorL.power=0; 				
    motorL.direction=1; 
    motorL.dutyHighByte=(unsigned char *)(&PWM6DCH); 
    motorL.dir_LAT=(unsigned char *)(&LATE); 	
    motorL.dir_pin=4; 	
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
        
        
        /******** Send Back Colour Readings to PC ******/
        if(timer_flag) { 
            get_int_status(); //Send back Interrupt Status
            read_All_Colors();
            SendColorReadings(); 
            timer_flag =0;  
        }
        
        
        if (!PORTFbits.RF2) {
            /*****Test Forward/Backward Movement ***/
            /*
            stop(&motorL, &motorR);
            move_forward(&motorL, &motorR,0);
            __delay_ms(1000);
            stop(&motorL, &motorR);
            
            __delay_ms(2000);
            move_backward(&motorL, &motorR,0);
            __delay_ms(2000);
            stop(&motorL, &motorR);
            */
            
            
            /***** Test Timed Forward/Backward Movement ***/
            /*
            stop(&motorL, &motorR);
            move_forward(&motorL, &motorR,1000);
            stop(&motorL, &motorR);
            
            __delay_ms(2000);
            move_backward(&motorL, &motorR,1000);
            stop(&motorL, &motorR);
            */
            
            /***** Test Turning - modify friction******/
            /*
             TurnLeft(mL,mR);
             TurnRight(mL,mR);
             
             */
            
            
            /**** Test friction variable is being updated ****/
            /*
             * 
            char fricstring[9];
            sprintf(fricstring,"  F:%d  ", friction);
            TxBufferedString(fricstring);
            sendTxBuf();
             * 
             */
            
            /******* Test Pickmove and Pointers *****/
            /*
             
             
             */
            
            /******** Test Timer0 Recording *******/
            /*
             setTMR0(0); //Start timer
             move_forward(&motorL, &motorR,1000);
             stop(&motorL, &motorR);
             unsigned int temp_test = getTMR0();
             
             
             
             */
            
        
        } 
        
        /************ Test actual operation ******************/
        /*
         * 
        // Start timer to time movement duration
        Timer0_init();
        move_forward(&motorL,&motorR,0);
        // Move forward until interrupt is triggered
        //When interrupt is triggered
        // - Stop Timer0
        // - Stop motor
        // - Read card
        // - Decide the color and execute the motion via pick_move
        // Then start Timer0 from zero and start moving again
        
        */
            
         
            
    }      
    
    
}