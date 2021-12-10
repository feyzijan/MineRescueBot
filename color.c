#include <xc.h>
#include "color.h"
#include "i2c.h"
#include <stdio.h>
#include "serial.h"

unsigned int blue_th = 1100; 
unsigned int green_th= 1800;
unsigned int red_th = 2500;


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
    //Configure interrupt thresholds RGBC clear channel: Low 300 High: 6950
    color_writetoaddr(0x04, 0x00); 
    color_writetoaddr(0x05, 0x00); 
    color_writetoaddr(0x06, 0xA0); 
    color_writetoaddr(0x07, 0x0F); 
    
    color_writetoaddr(0x0C, 0b0001); // Persistence register = 5
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


// Work in Progress
char decide_color(void){
    //char color;
    unsigned int redval, greenval, blueval, clearval;
   
    redval = color_read(0x16); 
    greenval = color_read(0x18); 
    blueval = color_read(0x1A); 
    //clearval = color_read(0x14); 
    
    //Procedure to decide
    // For testing 
    if(redval > 2500){
        sendCharSerial4('R');
        return 'r';
    } else if(blueval > 1000){
        sendCharSerial4('B');
        return 'b';
    }else if(blueval < 1000){
        sendCharSerial4('G');
        return 'g';
    } else{
        sendCharSerial4('O');
        return 'o'; 
    }
    
}