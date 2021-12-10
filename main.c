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


// Set correct friction value for 45^turns 


void main(void){
    
    // Variable Initialisations
    friction = 400; // Delay required ofor a 45^ turn - can be updated outside main
    reverse_time = 1500; // Delay required for Reverse One Square - has to be updated inside main 
    
    //Initialisations 
    initUSART4();
    color_click_init();
    LEDs_buttons_init();
    Interrupts_init();
    Timer1_init();
    
    
    //****Motor Initialisation****
  
    //Initialise Motor structs
    struct DC_motor motorL, motorR; 
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
    
    //setMotorPWM(&motorL);
    //setMotorPWM(&motorR);
           
    initDCmotorsPWM(199);     
    stop(&motorL,&motorR);
    
    //*** Test DC Motor Moves ***///
    //move_forward(&motorL,&motorR,1000);
    //stop(&motorL,&motorR);
    //move_backward(&motorL,&motorR,reverse_time);
    //TurnLeft(&motorL,&motorR);
    //__delay_ms(1000);
    //TurnRight(&motorL,&motorR);
 
    LightToggle(); 
    while(1){

        /******** Send Back Colour Readings to PC every 2 secs ******/
        if(timer_flag) { 
            __debug_break();
            get_int_status(); //Send back Interrupt Status
            read_All_Colors();
            SendColorReadings(); 
            timer_flag =0;  
        }
        
        // Re-enable clicker interrupt with button press
        if (ButtonRF2) { 
            color_click_interrupt_init();  
            __delay_ms(1);
        }  
        if (ButtonRF3) {
            LightToggle();     
            __delay_ms(1);
        }  

        /************ Test actual operation ******************/
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

//************* Crude Test Cases ***************//////


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