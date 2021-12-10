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



extern unsigned int timer0val;


void main(void){
    
    // Variable Initialisations
    friction = 600; 
    reverse_time = 200;
     // Delay required for Reverse One Square - has to be updated inside main 
    
    //Initialisations 
    initUSART4();
    color_click_init();
    LEDs_buttons_init();
    Interrupts_init();
    Timer1_init();
    
    
    //****Motor Initialisation****//
  
    //Initialise Motor structs
    struct DC_motor motorL,motorR;
    motorL.power=1; 	// For some reason when this is zero it breaks everything			
    motorL.direction=0; 
    motorL.dutyHighByte=(unsigned char *)(&PWM6DCH); 
    motorL.dir_LAT=(unsigned char *)(&LATE); 	
    motorL.dir_pin=4; 	
    motorL.PWMperiod=199; 
    
    motorR.power=1; 						
    motorR.direction=0; 					
    motorR.dutyHighByte=(unsigned char *)(&PWM7DCH);	
    motorR.dir_LAT=(unsigned char *)(&LATG); 		
    motorR.dir_pin=6;
    motorR.PWMperiod=199; 
    
    // Initialise pointer to structs
    struct DC_motor * mL = &motorL; 
    struct DC_motor * mR = &motorR; 
 
    initDCmotorsPWM(199);     
    stop(mL,mR);
    //********************//
    

    
    Timer0_init();
    
    //LightToggle(); 
    
    card_func my_function;
    
    while(1){
        
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
        
        
        //******************* Re-enable the clicker interrupt **************//
        /*
        if (ButtonRF2) { 
            color_click_interrupt_init();  
            __delay_ms(10);
        }
        */
        
        //******** Testing Function Pointer Memory **************//
        if (ButtonRF3){
            __delay_ms(100);
            add_function_ptr(&green_move);
            add_function_ptr(&red_move);
            my_function =  get_function_ptr();
            my_function(mL,mR); // executes green move (turn Left)
            my_function =  get_function_ptr();
            my_function(mL,mR); // executes red move (turn Right)
        }
        
        //************ Testing that Timer0 logs movement duration *************//
        
        if (ButtonRF2) { 
            __delay_ms(500); //So that button Press doesn't trigger condition below
            setTMR0(0); // Reset Timer0
            
            move_forward(&motorL,&motorR,0); // Move forward until button press
            while(!ButtonRF2); // move straight until interrupt
            
            getTMR0_in_ms(); // log time in memory
           
            stop(&motorL,&motorR); // Stop buggy
            __delay_ms(1000); // wait for a bit
            
            move_backward(&motorL,&motorR,get_timing()); // Retrieve timer from memory and move back
            
            stop(&motorL,&motorR);
        }
         

        /*
       while(!ButtonRF2);

       // Start timer to time movement duration
       Timer0_init();
       move_forward(&motorL,&motorR,0);
       while(!wall_flag); // move straight until interrupt
       // Now clicker interrupt has been triggerd - we are near a wall


       //Step 1: Stop motor
       stop(&motorL,&motorR);
       //Step 1.1: Add the movement time( read in interrupt) to our list
       add_timing(timer0val);
       //Step 2: Turn on Light
       //LightToggle();
       //Step 3: Take readings and decide on color
       color = decide_color();
       //Step 4: Pick and execute appropriate move
       pick_move(color,&motorL,&motorR);

       while(!ButtonRF2);
       
       */

    }
}


    //****************TODO: Calibration Routine for surface ****************//

    /*
    while(!(ButtonRF3 && ButtonRF2)) // Press both buttons to exit calibration
    {
        for(int k = 0; k<4; k++){ //Loop 180
            TurnLeft(&motorL,&motorR);
            __delay_ms(1000);
        }
    }
        */