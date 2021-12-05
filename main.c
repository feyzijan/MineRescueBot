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

unsigned char intstatus;


char moves[30];

char *pmoves = &moves;


char color;

void main(void){
    
    //Initialisations
    initUSART4(); 
    color_click_init();
    LEDsInit();
    Interrupts_init();
    Timer0_init();
    initDCmotorsPWM(199); 
    
    // Motor Initialisation
    //Initialise Motor structs
    struct DC_motor motorL, motorR; // declare two DC_motor structures 
    motorL.power=0; 				
    motorL.direction=1; // forward
    motorL.dutyHighByte=(unsigned char *)(&PWM6DCH); // store address of PWM duty high byte
    motorL.dir_LAT=(unsigned char *)(&LATE); 	// store address of LAT
    motorL.dir_pin=4; 	// pin RE4 controls direction
    motorL.PWMperiod=199; // store PWMperiod for motor

    //same for motorR but different PWM register, LAT and direction pin
    motorR.power=0; 						
    motorR.direction=1; 					
    motorR.dutyHighByte=(unsigned char *)(&PWM7DCH);	
    motorR.dir_LAT=(unsigned char *)(&LATG); 		
    motorR.dir_pin=6; 						//pin RG6 controls direction
    motorR.PWMperiod=199; 		

    
    //LightToggle();
    
    // Button RF2 - for debugging
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin  
   
    
    while(1){
        if (!PORTFbits.RF2) {
            LATBbits.LATB0 = 1; //Attempted to use button to trigger interrupt - doesn't work
        } 
        
        // Once every second
        if(timer_flag) { 
            read_All_Colors(); // Read RGBC values
            intstatus = get_int_status(); // for debugging: check interrupt status at 0x13: appears always on
            SendColorReadings(); // Send all color readings over Serial Port
            timer_flag =0;   
        } 
    }    
    
    
    stop(&motorL,&motorR);
    pick_move(color, *pmoves, &motorL,&motorR);
    pmoves = pmoves + 7; //move to the next char 
    
}