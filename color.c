#include <xc.h>
#include "color.h"
#include "i2c.h"
#include <stdio.h>
#include "serial.h"
#include "LEDs.h"


extern unsigned int int_low, int_high; // declared in color.h, defined in main.c


void color_click_init(void) {
  
  I2C_2_Master_Init();//Initialise I2C Master
	color_writetoaddr(0x00, 0x01); //set device PON to turn ColourClick on
  __delay_ms(3); //need to wait 3ms for everthing to start up
  color_writetoaddr(0x00, 0x03); // Turn on device ADC
  color_writetoaddr(0x01, 0xD5); //integration time: 42 Integ Cycles, 101ms, Max count:43008
  color_writetoaddr(0x0F, 0x00); // Gain: 1X
}   


void color_click_interrupt_init(void){
    //__debug_break();
    color_int_clear();
    color_writetoaddr(0x00, 0x13); //turn on Clicker Interrupt(write 1 to AIEN bit)

    //Configure interrupt thresholds RGBC clear channel
    // Use global variables that can be updated by the calibration function
    color_writetoaddr(0x04, int_low & 0xFF); 
    color_writetoaddr(0x05, int_low >> 8); 
    color_writetoaddr(0x06, int_high & 0xFF); 
    color_writetoaddr(0x07, int_high >>8);

    color_writetoaddr(0x0C, 0b0001); // Persistence register = 1
    color_int_clear();
}


// Update global variables for high and low (low is 0 by default has we don't want to trigger interrupt when light level falls) 
void interrupt_threshold_calibrate(void) {
    // Calibration procedure
    // start the procedure with button press
    unsigned int amb_and_LED;
    while (ButtonRF3); //empty while loop (wait for button press)
    for(int i=0;i<5;i++){   // indicate the procedure has started with 5 flashes of LED
        __delay_ms(100);
        LED2=1;
        __delay_ms(100);
        LED2=0;
    }
    clear = color_read(0x14); // read clear color channel for blue card
    int_high = clear;
    
    while (ButtonRF2); //empty while loop (wait for button press)
    for(int i=0;i<5;i++){   // indicate the procedure has started with 5 flashes of LED
        __delay_ms(100);
        LED2=1;
        __delay_ms(100);
        LED2=0;
    }
    clear = color_read(0x14); // read clear color channel for blue card
    amb_and_LED = clear;    
    
    while (ButtonRF3); //empty while loop (wait for button press)
    for(int i=0;i<5;i++){   // indicate the procedure has started with 5 flashes of LED
        __delay_ms(100);
        LED2=1;
        __delay_ms(100);
        LED2=0;
    }    
    clear = color_read(0x14); // read clear color channel for blue card
    
    if(clear<amb_and_LED){int_low=clear+(clear/20);}
    else{int_low=0;}

    // wait for a second button press to finish the calibration
    while (ButtonRF3); //empty while loop (wait for button press)
    for(int i=0;i<5;i++){ // indicate the procedure has ended
        __delay_ms(100);
        LED2=1;
        __delay_ms(100);
        LED2=0;
    }
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



/*Send the interrupt status over the Serial port
 Function for debugging purposes only - delete later*/ 
void get_int_status(void)
{  
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


//TESTING Shorten Below function
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


//Potential problem with two lists being the same?
char decide_color(void){
    char color_decision;
    
    __delay_ms(500); // allow readings to stabilize   
    read_All_Colors(); // read color channel values
  
    unsigned int LED_and_amb_read[4] = {red,green,blue,clear};// light from ambient + LED cross talk + LED reflection {red,green,blue,clear)
    LightToggle(); // turn RGB LED off 
    
    int black_thresh = clear;
    
    __delay_ms(500);                                // allow readings to stabilize
    read_All_Colors();                              // read color channel values
    unsigned int ambient[4] = {red,green,blue,clear};               // read color channel values
    
    
    // correct the readings to remove ambient and LED cross_talk
    red = LED_and_amb_read[0]-ambient[0]-LED_cross_talk[0];
    green = LED_and_amb_read[1]-ambient[1]-LED_cross_talk[1];
    blue = LED_and_amb_read[2]-ambient[2]-LED_cross_talk[2];
    clear = LED_and_amb_read[3]-ambient[3]-LED_cross_talk[3];
    
    
    // calculate integer percentage RGB values of clear channel
    int redPercentage = (100*red)/clear;
    int greenPercentage = (100*green)/clear;
    int bluePercentage = (100*blue)/clear;

    //Procedure to decide - will explain in read me file to follow logic process via graphs
    if (black_thresh <= int_low){color_decision=9;}
  
    // Red or Orange
    else if (redPercentage >= 65){
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
            color_decision = 7; // Light blue
        }
    }
    
    // Blue or green
    else if(redPercentage<35 && redPercentage >= 20){       
        if (bluePercentage>=29){
            color_decision = 3; // Blue
        }
        else{
            color_decision = 2; // Green
        }
    }   
  
    return color_decision; 
}

  // Function swe may use later
  /*
// read all colour channel values for an array
void color_read_all(unsigned int *colorArray)
{
    for(int i=0;i<4;i++){                           // read RGBC channel values
        *(colorArray+i) = color_read(0x14+2*i);
    }
}
*/
