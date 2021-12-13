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


extern unsigned int timer0val; // For logging movement time

void main(void){
    
    // Move these to separate files for final build
    // Global Variable Initialisations 
    friction = 130; //45 degree turn time
    reverse_time = 2100; // Reverse_square time
     
    //Initialisations 
    color_click_init();
    LEDs_buttons_init();
    Timer0_init();
    interrupt_threshold_calibrate();
    Interrupts_init();
    
    
    //********************** Motor Initialisation ****************************//
                                                    
    //Initialise Motor structs and pointers 
    struct DC_motor motorL,motorR;
    struct DC_motor * mL = &motorL; 
    struct DC_motor * mR = &motorR; 
    motorL.power=1; 	// For some reason when this is zero it breaks everything			
    motorL.direction=1; 
    motorL.dutyHighByte=(unsigned char *)(&PWM6DCH); 
    motorL.dir_LAT=(unsigned char *)(&LATE); 	
    motorL.dir_pin=4; 	
    motorL.PWMperiod=199; 
    
    motorR.power=1; 						
    motorR.direction=1; 					
    motorR.dutyHighByte=(unsigned char *)(&PWM7DCH);	
    motorR.dir_LAT=(unsigned char *)(&LATG); 		
    motorR.dir_pin=6;
    motorR.PWMperiod=199; 
 
    initDCmotorsPWM(199); //Initialise PWM module  
    stop(mL,mR);
    
    //************************************************************************//
  
    // Calibration Functions - Detailed instructions in header files
    //CalibrateTurns(mL,mR);
    //__delay_ms(1000);
    //CalibrateReverseSquare(mL,mR);
    //__delay_ms(1000);
       
    // Initialisation for testing
    initUSART4(); 
    Timer1_init();
    
    unsigned char color;
   
    //LightOn(); 
    while(1){
    
      
        //if (ButtonRF2) {LightOn();} // turn RGB light off manually if required
        if (ButtonRF3) {  // Turn on interrupt source manually for testing
          color_click_interrupt_init();
          BrakeLight=0;
       
        }
        
        
        if (color_flag){
            color = decide_color();
            LED1 = 0;
            for (int i=0;i<color;i++){
                LED1 = 1;
                __delay_ms(350);
                LED1 = 0;
                __delay_ms(350);
            }
            color = 0;
            BrakeLight = 0;
            color_flag = 0;
                }
       
        
        
        //************************ Main Operating Loop ************************//
        
        
        if(ButtonRF2){// Wait for Button press to start - For Testing
           //LightOn(); //LED on
           //color_click_interrupt_init();
           //!end_motion
           while(1){ // Use flag that is set to 1 with final card

                // Step 1: Forward Motion
                //BrakeLight = 0;
                //ResetTMR0();//Start timer to time movement duration
                //move_forward(mL,mR,0); // Move forward
                while(!color_flag); // Continue motion until clicker triggers this flag
                
                //LightTest();
                //Step 2: Stop buggy and read card
                //stop(mL,mR); // Interrupt means we are near a card so stop
                color = decide_color(); // Logic process to decide color 
                if (color_flag){
                    LED1 = 0;
                    for (int i=0;i<color;i++){
                        LED1 = 1;
                        __delay_ms(350);
                        LED1 = 0;
                        __delay_ms(350);
                    }
                    color = 0;
                    color_flag = 0;
                    BrakeLight = 0;
                }

                //Step 3: Pick and execute appropriate move
                //pick_move(color, mL,mR); // Execute needed motion and update motion memory

                //Step 4: Re-enable clicker interrupt 
                LightOn();
                color_click_interrupt_init();
            }
        } 
       
       
       //********************************************************************//
        
        
        //****************** Indicate Colour Reading with LED *****************//
        /*
        
        */ 
        //********************************************************************//
    }
}


// Test Code
/******** Send Back Colour Readings to PC every 2 secs **********/
        /*
        if(timer_flag) { 
            
            get_int_status(); //Send back Interrupt Status
            read_All_Colors();
            SendColorReadings(); 

            reset_Timer0();
            timer_flag =0;  
            
        }
 */
