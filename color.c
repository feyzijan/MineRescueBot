#include <xc.h>
#include "color.h"
#include "i2c.h"

void color_click_init(void)
{   
    I2C_2_Master_Init();//Initialise i2c Master

	color_writetoaddr(0x00, 0x01); //set device PON to turn ColourClick on
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
	color_writetoaddr(0x00, 0x03); //turn on device ADC

	color_writetoaddr(0x01, 0xD5); //set integration time
    //42 Integ Cycles, 101ms, 43008 max count

    //color_writetoaddr(0x00, 0x13); // RGBC interrupt enable

    color_writetoaddr(0x03, 0x00); // set wait time register
    
    color_writetoaddr(0x0F, 0x01); // Set gain to 4x?
    
}

void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode (note 0x52=0x29<<1)
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}

unsigned int color_read_Red(void)
{  
    //__debug_break();
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	
    // Edit line below to read different channels
    I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	
    I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	
    tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
    //__debug_break();
}



