#include <xc.h>
#include "color.h"
#include "i2c.h"
#include "LEDs.h"
#include <stdio.h>
#include "serial.h"
#include <math.h>


void color_click_init(void)
{   
    I2C_2_Master_Init();//Initialise i2c Master

	color_writetoaddr(0x00, 0x01); //set device PON to turn ColourClick on
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
	color_writetoaddr(0x00, 0x03); // Turn on device ADC

    // TO CONFIGURE WITH TESTING
	color_writetoaddr(0x01, 0xD5); //integration time: 64 Integ Cycles, 154ms, 65535 max count

    
    //color_writetoaddr(0x00,0x1B); // Enable Wait time register?
    //color_writetoaddr(0x03, 0x00); // Wait time register: 256
    
    color_writetoaddr(0x0F, 0x00); // Gain: 4X
    
    //color_writetoaddr(0x0C, 0x00);
}   


void color_click_interrupt_init(void){
    //__debug_break();
    color_int_clear();
    color_writetoaddr(0x00, 0x13); //turn on Clicker Interrupt(write 1 to AIEN bit)
    //Configure interrupt thresholds RGBC clear channel: Low 300 High: 1250
    color_writetoaddr(0x04, 0x2C); 
    color_writetoaddr(0x05, 0x01); 
    color_writetoaddr(0x06, 0xD6); 
    color_writetoaddr(0x07, 0x06); 
    
    color_writetoaddr(0x0C, 0b0001); // Persistence register = 5
    color_int_clear();
}

void color_click_interrupt_off(void){
    color_int_clear();
    color_writetoaddr(0x00,0x03);
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
/*
// read all colour channel values for an array
void color_read_all(unsigned int *colorArray)
{
    for(int i=0;i<4;i++){                           // read RGBC channel values
        *(colorArray+i) = color_read(0x14+2*i);
    }
}
*/

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


// Work in Progress
int decide_color(void){
    int color_decision = 100;
    
    //unsigned int LED_and_amb_read[4] = {0,0,0,0};   // light from ambient + LED cross talk + LED reflection {clear,red,green,blue)
    //unsigned int ambient[4] = {0,0,0,0};            // light from ambient alone (when LED is turned off)
    
    __delay_ms(500);                                // allow readings to stabilize   
    read_All_Colors();                              // read color channel values
    unsigned int LED_and_amb_read[4] = {red,green,blue,clear};      // light from ambient + LED cross talk + LED reflection {red,green,blue,clear)
    LightToggle();                                  // turn RGB LED off   
    __delay_ms(500);                                // allow readings to stabilize
    read_All_Colors();                              // read color channel values
    unsigned int ambient[4] = {red,green,blue,clear};               // read color channel values
    
    
    // correct the readings to remove ambient and LED cross_talk
    red = LED_and_amb_read[0]-ambient[0]-LED_cross_talk[0];
    green = LED_and_amb_read[1]-ambient[1]-LED_cross_talk[1];
    blue = LED_and_amb_read[2]-ambient[2]-LED_cross_talk[2];
    clear = LED_and_amb_read[3]-ambient[3]-LED_cross_talk[3];
    
    
    // calculate integer percentage RGB values of clear channel
    int redPercentage = 100*(red*pow(clear,-1));
    int greenPercentage = 100*(green*pow(clear,-1));
    int bluePercentage = 100*(blue*pow(clear,-1));

    unsigned int red_channel_red_orange_sep_thresh = 200; // a threshold to distinguish between red and orange in decision process
    unsigned int clear_channel_black_thresh = 100;        // a threshold to tell if the color is black

    //Procedure to decide
    
    if (redPercentage >= 65){
        if (red > red_channel_red_orange_sep_thresh){
            //color_decision = 'r';
            color_decision = 1;
        }
        else{
            //color_decision = 'o';
            color_decision = 6;
        }
    }
    else if(redPercentage < 65 && redPercentage >= 52){
        if (greenPercentage>30 && bluePercentage<21){
            //color_decision = 'y';
            color_decision = 4;
        }
        else{
            //color_decision = 'p';
            color_decision = 5;
        }
    }
    else if(redPercentage < 52 && redPercentage >= 35){
        if (clear<clear_channel_black_thresh){
            //color_decision = 'b';
            color_decision = 9;
        }
        else if (redPercentage>=45){
            //color_decision = 'w';
            color_decision = 8;
        }
        else{
            //color_decision = 'l';
            color_decision = 7;
        }
    }
    else if(redPercentage<35 && redPercentage >= 20){
        if (bluePercentage>=29){
            //color_decision = 'b';
            color_decision = 3;
        }
        else{
            //color_decision = 'g';
            color_decision = 2;
        }
    }
    //color_decision = 7;
    return color_decision;
 
}
