#include <xc.h>
#include "color.h"
#include "i2c.h"
#include <stdio.h>
#include "serial.h"
#include "LEDs.h"
#include "interrupts.h"

extern unsigned int int_low, int_high; // declared in color.h, defined in main.c


void color_click_init(void) {
    
    I2C_2_Master_Init(); // Initialise I2C Master
    color_writetoaddr(0x00, 0x01); // Set device PON to turn ColourClick on
    __delay_ms(3); // Need to wait 3ms for everything to start up
    color_writetoaddr(0x00, 0x03); // Turn on device ADC
    color_writetoaddr(0x01, 0xD5); // Integration time: 42 Integ Cycles, 101ms, Max count:43008
    color_writetoaddr(0x0F, 0x00); // Gain: 1X
}   



void color_writetoaddr(char address, char value){
    I2C_2_Master_Start(); 
    I2C_2_Master_Write(0x52 | 0x00); //7 bit device address + Write mode (0x52=0x29<<1)
    I2C_2_Master_Write(0x80 | address); //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop(); //Stop condition
}



void color_click_interrupt_init(void){
    
    color_int_clear(); // Clear Interrupt Flag 
    color_writetoaddr(0x00, 0x13); //turn on Clicker Interrupt(write 1 to AIEN bit)
    
    // Configure interrupt threshold for clear channel with calibrate values
    color_writetoaddr(0x04, int_low & 0xFF);      // Low threshold low byte
    color_writetoaddr(0x05, int_low >> 8);        // Low threshold high byte
    color_writetoaddr(0x06, int_high & 0xFF);     // High threshold low byte
    color_writetoaddr(0x07, int_high >>8);        // High threshold high byte
    color_writetoaddr(0x0C, 0b0001);              // Persistence register = 1 
    
    color_int_clear(); 
}



void interrupt_threshold_calibrate(void) {
    
    LightTest(); // Toggle Light to indicate start of Calibration
    
    __delay_ms(500);
    LightOn();
    unsigned int amb_and_LED;
    
    while (!ButtonRF3); //Wait for button press)
    LightTest();  // Indicate procedure has started
    clear = color_read(0x14); // Read clear channel for blue card
    int_high = clear;
    
    while (!ButtonRF3); 
    LightTest(); 
    clear = color_read(0x14); // Read clear channel for ambient
    amb_and_LED = clear;    
    
    while (!ButtonRF3); 
    LightTest(); 
    clear = color_read(0x14); // Read clear channel for black card
    
    // Assign low threshold to ambient or black reading, whichever lowest
    if(clear<amb_and_LED){int_low=clear+(clear/20);}
    else{int_low=0;}

    while (!ButtonRF3); // Wait for button press to exit calibration   
    LightTest(); // Toggle Light to indicate end of Calibration
}


void color_click_interrupt_off(void){
    color_int_clear(); // Clear Interrupt just in case
    color_writetoaddr(0x00,0x03); // Disable Interrupt
    color_int_clear();
}


void color_int_clear(void){
    I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
    I2C_2_Master_Write(0xE6); // Clear RGBC interrupt
    I2C_2_Master_Stop();
}


unsigned int color_read(unsigned char address)
{  
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
    I2C_2_Master_Write(0xA0 | address);//command (auto-increment protocol transaction) 
    //+ start at a colour's low register

    I2C_2_Master_RepStart();// start a repeated transmission
    I2C_2_Master_Write(0x52 | 0x01);//7 bit address + Read (1) mode

    tmp=I2C_2_Master_Read(1);//read the  LSB
    tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the MSB (don't acknowledge as this is the last read)
    I2C_2_Master_Stop();      
    return tmp;
}


void read_All_Colors(void){
    red = color_read(0x16); 
    green = color_read(0x18); 
    blue = color_read(0x1A); 
    clear = color_read(0x14); 
}


//Potential problem with two lists being the same?
char decide_color(void){
    
    char color_decision;
    unsigned int black_thresh;
    
    LightOn(); // Turn Led on for first reading
    __delay_ms(500); // Wait for readings to stabilise   
    read_All_Colors();
  
    // Array of readings: Light from ambient + LED cross talk + LED reflection 
    unsigned int LED_and_amb_read[4] = {red,green,blue,clear};  
    black_thresh = clear; // Prospective threshold
    
    LightOff(); // Turn LED off 
    __delay_ms(500);
    read_All_Colors();
    // Array of readings: Ambient Only
    unsigned int ambient[4] = {red,green,blue,clear};
    
  
    // Correct the readings to remove ambient and LED cross_talk
    red = LED_and_amb_read[0]-ambient[0]-LED_cross_talk[0];
    green = LED_and_amb_read[1]-ambient[1]-LED_cross_talk[1];
    blue = LED_and_amb_read[2]-ambient[2]-LED_cross_talk[2];
    clear = LED_and_amb_read[3]-ambient[3]-LED_cross_talk[3];
    
    // Calculate percentage of RGB channel values (as percentage of clear channel)
    int redPercentage = (100*red) / clear;
    int greenPercentage = (100*green) / clear;
    int bluePercentage = (100*blue) / clear;

    //Color Decision Procedure based on Percentage composition of readings
    
    // Check black first
    if (black_thresh <= int_low){
        color_decision=9;
    } // Red or Orange  
    else if (redPercentage >= 65){ 
        if(greenPercentage<11){
            color_decision = 1;  //RED
        } else{
            color_decision = 6; // Orange
        } 
    } // Yellow or Pink
    else if(redPercentage < 65 && redPercentage >= 52){ 
        if (greenPercentage>30 && bluePercentage<21){
            color_decision = 4; // Yellow
        }else{
            color_decision = 5; // Pink
        }
    } // White or light blue
    else if(redPercentage < 52 && redPercentage >= 35){ 
        if (redPercentage>=45){
            color_decision = 8; // White
        } else{
            color_decision = 7; // Light blue
        }
    }
    else if(redPercentage<35 && redPercentage >= 20){   // Blue or green    
        if (bluePercentage>=29){
            color_decision = 3; // Blue
        }else{
            color_decision = 2; // Green
        }
    }   
    return color_decision; 
}

/******************Testing Functions***********************************/


void get_int_status(void)
{  
  	unsigned char tmp;
    unsigned char intstatus[9];
    unsigned char throw;
    I2C_2_Master_Start();       
    I2C_2_Master_Write(0x52 | 0x00);     
    I2C_2_Master_Write(0xA0 | 0x13);
    I2C_2_Master_RepStart();
    I2C_2_Master_Write(0x52 | 0x01);
    tmp = I2C_2_Master_Read(1);
    throw = I2C_2_Master_Read(0); 
    I2C_2_Master_Stop();
    tmp = tmp >>4;
    sprintf(intstatus, "  INT:%d  ",tmp );
    TxBufferedString(intstatus);
    sendTxBuf();
}


void Color2String(char *ptr, unsigned int *pval){
    sprintf(ptr,"  %05d ", *pval);
}


void SendColorReadings(void){
    char colorstring[8];
    putCharToTxBuf(' ');
    sprintf(colorstring," R%05d ", red);
    TxBufferedString(colorstring); 
    sprintf(colorstring," G%05d ", green);
    TxBufferedString(colorstring); 
    sprintf(colorstring," B%05d ", blue);
    TxBufferedString(colorstring); 
    sprintf(colorstring," C%05d ", clear);
    TxBufferedString(colorstring);
    sendTxBuf(); //interrupt will handle the rest
}

