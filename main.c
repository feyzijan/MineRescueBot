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
#include "LEDsButtons.h"
#include "timers.h"
#include "interrupts.h"
#include "CardMoves.h"
#include "Memory.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void main(void){
    
    //Initialisations 
    color_click_init();
    LEDs_Buttons_init();

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
    
    //**** Calibration Functions - Detailed instructions in header files *****//
    interrupt_threshold_calibrate();
    __delay_ms(1000);
    CalibrateTurns(mL,mR);
    __delay_ms(1000);
    CalibrateReverseSquare(mL,mR);
    __delay_ms(1000);
    //************************************************************************//
       
    LightOn();
    Interrupts_init();
     
    char color_detected = 0;
    while(1){
    
        // Manually Re-enable interrupt while checking Calibration
        if (ButtonRF3) color_click_interrupt_init(); 
        
        //************************ Main Operating Loop ************************//
        if(ButtonRF2){// Press Button to start Mission!
           
           color_click_interrupt_off();  
           LightOn();
           color_click_interrupt_init(); // Re-initialise interrupt
           
           color_flag = 0; // Set flags to 0
           lost_flag = 0;
           Timer0_init();
           
           
           while(color_detected <8){ // Not told to go home in any way
                // Step 1: Forward Motion
                ResetTMR0();// Start timer to time movement duration
                move_forward(mL,mR,0); // Move forward
                
                // Continue motion until an interrupt is raised and a flag set
                while(!color_flag && !lost_flag); 
                            
                if(color_flag){ // Clicker Interrupt: Read Card and Move On
                    
                    //Step 2: Stop buggy and read card
                    stop(mL,mR);
                    __delay_ms(250); // Wait for readings to stabilise
                    color_detected = decide_color();
                    color_flag = 0; 

                    //Step 3: Pick and execute appropriate move & update memory
                    pick_move(color_detected, mL,mR); 

                    //Step 4: Re-enable clicker interrupt and move on
                    color_click_interrupt_init();
            
                } else {// LostFlag set via TMR0 overflow or memory overflow
                    stop(mL,mR);
                    getTMR0_in_ms(); // Log movement duration in memory
                    color_click_interrupt_off(); // Turn off (and clear) clicker interrupt
                    pick_move(8, mL,mR);  // Buggy lost- go home
                } 
            }
        }    
    }
}
