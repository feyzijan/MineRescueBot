#include <xc.h>
#include "color.h"

// Prospective value that tends to work well
unsigned int int_high = 3250; 
unsigned int int_low = 0;

void color_click_init(void) {
    
    I2C_2_Master_Init();             // Initialise I2C Master
    color_writetoaddr(0x00, 0x01);   // Set device PON to turn ColourClick on
    __delay_ms(3);                   // Wait 3ms for everything to start up
    color_writetoaddr(0x00, 0x03);   // Turn on device ADC
    color_writetoaddr(0x01, 0xD5);   // Integ. time: 42 Cycles, 101ms, Max:43008
    color_writetoaddr(0x0F, 0x00);   // Gain: 1X
}   


void color_click_interrupt_init(void){
    
    color_int_clear();             // Clear Interrupt Flag  
    color_writetoaddr(0x00, 0x13); // Enable Interrupt(write 1 to AIEN bit)
    // Configure interrupt threshold for clear channel with calibrate values
    color_writetoaddr(0x04, int_low & 0xFF);      // Low threshold low byte
    color_writetoaddr(0x05, int_low >> 8);        // Low threshold high byte
    color_writetoaddr(0x06, int_high & 0xFF);     // High threshold low byte
    color_writetoaddr(0x07, int_high >>8);        // High threshold high byte
    color_writetoaddr(0x0C, 0b0001);              // Persistence register = 3 
    color_int_clear(); 
    BrakeLight = 0; // Signify buggy is not in an interrupt state
}


void color_click_interrupt_off(void){
    color_int_clear(); // Clear Interrupt just in case
    color_writetoaddr(0x00,0x03); // Disable Interrupt
    color_int_clear();
}


void color_int_clear(void){
    I2C_2_Master_Start();            // Start condition
	I2C_2_Master_Write(0x52 | 0x00); // 7 bit address + Write mode
    I2C_2_Master_Write(0xE6);        // Clear RGBC interrupt
    I2C_2_Master_Stop();
}


void interrupt_threshold_calibrate(void) {
    
    LightsToggle();             // Indicate start of Calibration
    LightOn();
    unsigned int amb_and_LED;
    unsigned int black;
    
    while (!ButtonRF3);          // Wait for button press
    LightsToggle();              // Indicate reading is being taken
    int_high = color_read(0x14); // Read clear channel for blue card
    
    while (!ButtonRF3); 
    LightsToggle(); 
    amb_and_LED = color_read(0x14); // Read clear channel for ambient

    while (!ButtonRF3); 
    LightsToggle(); 
    black = color_read(0x14);       // Read clear channel for black card
    
    // If black card registers a lower value, set that + 5% tolerance as int_low
    // If not don't set a lower threshold
    if(black < amb_and_LED){
        int_low= black - black/20;
    } else{int_low=0;}
    
    while (!ButtonRF3); // Wait for button press to exit calibration   
    LightsToggle(); // Toggle Light to indicate end of Calibration
}


void color_writetoaddr(char address, char value){
    I2C_2_Master_Start(); 
    I2C_2_Master_Write(0x52 | 0x00); // 7bit device address + Write mode(0x52=0x29<<1)
    I2C_2_Master_Write(0x80 | address); // Command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();                // Stop condition
}


unsigned int color_read(unsigned char address){  
    
	unsigned tmp;
	I2C_2_Master_Start();                // Start condition
	I2C_2_Master_Write(0x52 | 0x00);     // 7 bit address + Write mode
    
    // Command (auto-increment protocol transaction) + start at a colour's low register
    I2C_2_Master_Write(0xA0 | address);  
    
    I2C_2_Master_RepStart();             // Start a repeated transmission
    I2C_2_Master_Write(0x52 | 0x01);     // 7 bit address + Read (1) mode
    tmp=I2C_2_Master_Read(1);            // Read LSB
    tmp=tmp | (I2C_2_Master_Read(0)<<8); // Read MSB (don't acknowledge as this is the last read)
    I2C_2_Master_Stop();      
    return tmp;
}


void read_All_Colors(unsigned int *writeArray){
    for(int i=0;i<4;i++){ 
        *(writeArray+i) = color_read(0x14+2*i); 
	}
}


char decide_color(void){
    // Initialise temporary local variables
    char color_decision;
    unsigned int black_threshold;
    unsigned int LED_and_ambient[4];
    unsigned int ambient[4];
    
    // First get readings with LED on
    LightOn(); 
    __delay_ms(500); // Wait for readings to stabilise   
    read_All_Colors(LED_and_ambient);
    black_threshold = LED_and_ambient[0]; 
    
    // Now get readings with LED off
    LightOff(); 
    __delay_ms(500);
    read_All_Colors(ambient);
    
    // Correct readings to remove ambient and LED cross_talk (constants below)
    // These cross talk values are constant for 1x Gain and 42 Integ. Cycles 
    // These are __int24 as they need to be multiplied by 100 below for % values
    __int24 clear_real = LED_and_ambient[0]- ambient[0]- 993;
    __int24 red_real   = LED_and_ambient[1]- ambient[1]- 484;
    __int24 green_real = LED_and_ambient[2]- ambient[2]- 268;
    __int24 blue_real  = LED_and_ambient[3]- ambient[3]- 183;    

    // Calculate RGB values as percentage of clear channel
    char redPercentage = (100*red_real)/ clear_real;
    char greenPercentage = (100*green_real) / clear_real;
    char bluePercentage = (100*blue_real) / clear_real;

    // Implement Decision Procedure
    if (black_threshold <= int_low){ // Black
       color_decision=9; 
    } else if (redPercentage >= 65){ // Red or Orange
        if(greenPercentage < 11){
            color_decision = 1;  //Red
        } else {
            color_decision = 6; // Orange
        } 
    } else if(redPercentage < 65 && redPercentage >= 52){ // Yellow or Pink
        if (greenPercentage >=29 && bluePercentage < 21){ 
            color_decision = 4; // Yellow
        } else { 
            color_decision = 5; // Pink
        }
    } else if(redPercentage < 52 && redPercentage >= 35){ // White or light blue
        if (redPercentage >= 45){
            color_decision = 8; // White
        } else {
            color_decision = 7; // Light blue
        }
    } else if(redPercentage <35 && redPercentage >= 15){  
        if (bluePercentage>=30){ 
            color_decision = 3; // Blue
        } else {
            color_decision = 2; // Green
        }
    } else{
        return 0; // No colour detected
    }
    LightOn();
    return color_decision; 
}


