#include <xc.h>
#include "i2c.h"


void I2C_2_Master_Init(void) {
  SSP2CON1bits.SSPM= 0b1000;               // I2C master mode
  SSP2CON1bits.SSPEN = 1;                  // Enable I2C
  SSP2ADD = (_XTAL_FREQ/(4*_I2C_CLOCK))-1; // Baud rate divider bits (master mode)
  
  //pin configuration for I2C 
  TRISDbits.TRISD5 = 1;                   // Disable output driver
  TRISDbits.TRISD6 = 1;                   // Disable output driver
  ANSELDbits.ANSELD5=0;
  ANSELDbits.ANSELD6=0;
  SSP2DATPPS=0x1D;                        // Pin RD5
  SSP2CLKPPS=0x1E;                        // Pin RD6
  RD5PPS=0x1C;                            // Data output
  RD6PPS=0x1B;                            // Clock output
}


void I2C_2_Master_Idle(void){
    while ((SSP2STAT & 0x04) || (SSP2CON2 & 0x1F)); 
}


void I2C_2_Master_Start(void){
  I2C_2_Master_Idle();    
  SSP2CON2bits.SEN = 1;           
}


void I2C_2_Master_RepStart(void){
  I2C_2_Master_Idle();
  SSP2CON2bits.RSEN = 1;          
}


void I2C_2_Master_Stop(){
  I2C_2_Master_Idle();
  SSP2CON2bits.PEN = 1;          
}


void I2C_2_Master_Write(unsigned char data_byte){
  I2C_2_Master_Idle();
  SSP2BUF = data_byte;        
}


unsigned char I2C_2_Master_Read(unsigned char ack){
  unsigned char tmp;
  I2C_2_Master_Idle();
  SSP2CON2bits.RCEN = 1;        // put the module into receive mode
  I2C_2_Master_Idle();
  tmp = SSP2BUF;                //Read data from SS2PBUF
  I2C_2_Master_Idle();
  SSP2CON2bits.ACKDT = !ack;     // 0 turns on acknowledge data bit
  SSP2CON2bits.ACKEN = 1;        //start acknowledge sequence
  return tmp;
}

