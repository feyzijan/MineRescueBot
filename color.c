#include <xc.h>
#include "color.h"
#include "i2c.h"
#include <stdio.h>
#include "serial.h"
#include "LEDs.h"


extern unsigned int int_low, int_high; // declared in color.h, defined in main.c

void color_click_init(void)
{   
    I2C_2_Master_Init();//Initialise i2c Master

	color_writetoaddr(0x00, 0x01); //set device PON to turn ColourClick on
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
	color_writetoaddr(0x00, 0x03); // Turn on device ADC

    // TO CONFIGURE WITH TESTING
	color_writetoaddr(0x01, 0xC0); //integration time: 64 Integ Cycles, 154ms, 65535 max count

    
    //color_writetoaddr(0x00,0x1B); // Enable Wait time register?
    color_writetoaddr(0x03, 0x00); // Wait time register: 256
    
    color_writetoaddr(0x0F, 0x00); // Gain: 1x
    
    //color_writetoaddr(0x0C, 0x00);
}   


void color_click_interrupt_init(void){
    //__debug_break();
    color_int_clear();
    color_writetoaddr(0x00, 0x13); //turn on Clicker Interrupt(write 1 to AIEN bit)
    //Configure interrupt thresholds RGBC clear channel:
    //color_writetoaddr(0x04, 0x00); 
    //color_writetoaddr(0x05, 0x00); 
    //color_writetoaddr(0x06, 0xA0); 
    //color_writetoaddr(0x07, 0x0F); 
    // Use global variables that can be updated by the calibration function
    color_writetoaddr(0x04, int_low & 0xFF); 
    color_writetoaddr(0x05, int_low >> 8); 
    color_writetoaddr(0x06, int_high & 0xFF); 
    color_writetoaddr(0x07, int_high >>8); 
    
    color_writetoaddr(0x0C, 0b0001); // Persistence register = 1
    color_int_clear();
}

void color_click_interrupt_off(void){
    color_int_clear(); //clear interrupt just in case
    color_writetoaddr(0x00, 0x03); //turn off Clicker Interrupt(write 0 to AIEN bit)
}


void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode (note 0x52=0x29<<1)
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}


void color_int_clear(void){
    I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
    I2C_2_Master_Write(0xE6); // Clear RGBC interrupt
}


unsigned int color_read(unsigned char address)
{  
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode

    I2C_2_Master_Write(0xA0 | address);//command (auto-increment protocol transaction) + start at a colour's low register

    I2C_2_Master_RepStart();// start a repeated transmission
    I2C_2_Master_Write(0x52 | 0x01);//7 bit address + Read (1) mode

    tmp=I2C_2_Master_Read(1);//read the  LSB
    tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the MSB (don't acknowledge as this is the last read)
    I2C_2_Master_Stop();      
    return tmp;
}


/*Send the interrupt status over the Serial port
 Function for debugging purposes only - delete later*/ 
void get_int_status(void)
{  
    //__debug_break();
	unsigned char tmp;
    unsigned char intstatus[9];
    unsigned char throw;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode

    I2C_2_Master_Write(0xA0 | 0x13);//command (auto-increment protocol transaction) + start at a colour's low register

    I2C_2_Master_RepStart();// start a repeated transmission
    I2C_2_Master_Write(0x52 | 0x01);//7 bit address + Read (1) mode

    tmp = I2C_2_Master_Read(1);//read the  LSB
    throw = I2C_2_Master_Read(0); //read the MSB (don't acknowledge as this is the last read)
    I2C_2_Master_Stop();
    tmp = tmp >>4;
    sprintf(intstatus, "  INT:%d  ",tmp );
    TxBufferedString(intstatus);
    sendTxBuf();
}


void Color2String(char *ptr, unsigned int *pval){
    sprintf(ptr,"  %05d ", *pval);
}


//TODO: Shorten Below function
void SendColorReadings(void){
    char colorstring[8];

    putCharToTxBuf(' ');
    
    //putCharToTxBuf('R');
    sprintf(colorstring," R%05d ", red);
    TxBufferedString(colorstring); 
    
    //putCharToTxBuf('G');
    sprintf(colorstring," G%05d ", green);
    TxBufferedString(colorstring); 
    
    //putCharToTxBuf('B');
    sprintf(colorstring," B%05d ", blue);
    TxBufferedString(colorstring); 
    
    //putCharToTxBuf('C');
    sprintf(colorstring," C%05d ", clear);
    TxBufferedString(colorstring);

    sendTxBuf(); //interrupt will handle the rest
}


void read_All_Colors(void){
    red = color_read(0x16); 
    green = color_read(0x18); 
    blue = color_read(0x1A); 
    clear = color_read(0x14); 
}


// Just for testing
char decide_color_test(void){
    unsigned int redval, greenval, blueval, clearval;
   
    redval = color_read(0x16); 
    greenval = color_read(0x18); 
    blueval = color_read(0x1A); 
    
    return 'r';
}


// We call this function in main.c
void interrupt_threshold_calibrate(void) {

    while (!ButtonRF3); //wait for button press
    __delay_ms(500); // Button delay
    LightTest(); // Indicate start
   
    clear = color_read(0x14); // read clear color channel for blue card
    
    // Update global variable high threshold 
    //int_high = 1000;
    
    while (!ButtonRF3); //wait for second button press
    LightTest(); // Indicate end
}


/*Changes to ajays code
 * Use char not int
 * Put own LightToggle function
 * 
*/

//Potential problem with two lists being the same?
char decide_color(void){
    
    char color_decision;
    __delay_ms(500); // wait for readings to stabilise  
        
    // Read with LED on
    read_All_Colors();
    unsigned int LED_and_amb_read[4] = {red,green,blue,clear}; // light from ambient + LED cross talk + LED reflection {red,green,blue,clear)
    
    // Read with LED off
    LightToggle();// turn RGB LED off 
    __delay_ms(500); // wait for readings to stabilise  
    read_All_Colors();
    unsigned int ambient[4] = {red,green,blue,clear};// light from ambient only
    
    
    // correct the readings to remove ambient and LED cross_talk
    red = LED_and_amb_read[0]- ambient[0] - LED_cross_talk[0];
    green = LED_and_amb_read[1]- ambient[1] - LED_cross_talk[1];
    blue = LED_and_amb_read[2]- ambient[2] - LED_cross_talk[2];
    clear = LED_and_amb_read[3]- ambient[3] - LED_cross_talk[3];
    
    
    // Calculate percentage RGB values of clear channel
    char redPercentage = (100*red)/clear;
    char greenPercentage = (100*green)/clear;
    char bluePercentage = (100*blue)/clear;

    //unsigned int red_channel_red_orange_sep_thresh = 200; // a threshold to distinguish between red and orange in decision process
    //unsigned int clear_channel_black_thresh = 2200;        // a threshold to tell if the colour is black

    //Procedure to decide - will explain in read me file to follow logic process via graphs
    
    // Either Red or Orange
    if (redPercentage >= 65){ 
        /*
        if (red > red_channel_red_orange_sep_thresh){
            //color_decision = 'r';
            color_decision = 1;
        }
         */
        if(greenPercentage<11){
            color_decision = 1; //RED
        }
        else{
            color_decision = 6; // Orange
        }
    }
    
    // Yellow or Pink
    else if(redPercentage < 65 && redPercentage >= 52){ 
        if (greenPercentage>30 && bluePercentage<21){
            color_decision = 4; // Yellow
        }
        else{
            color_decision = 5; // Pink
        }
    }
    
    // White or light blue
    else if(redPercentage < 52 && redPercentage >= 35){
        if (redPercentage>=45){
            color_decision = 8; // White
        }
        else{
            color_decision = 7; //Light blue
        }
    }
    
    // Blue or green
    else if(redPercentage<35 && redPercentage >= 20){       
        if (bluePercentage>=29){
            color_decision = 3; //Blue
        }
        else{
            color_decision = 2; //Green
        }
    }
    
    return color_decision; // return the specific color value
 
}