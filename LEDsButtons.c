#include <xc.h>
#include "LEDsButtons.h"


void LEDs_Buttons_init(void){
    //Set all LED pins as output
    TRISGbits.TRISG0 = 0; 
    TRISEbits.TRISE7 = 0; 
    TRISAbits.TRISA3 = 0; 
    TRISDbits.TRISD7 = 0;
    TRISHbits.TRISH3 = 0;
    TRISHbits.TRISH1 = 0; 
    TRISDbits.TRISD3 = 0; 
    TRISDbits.TRISD4 = 0; 
    TRISHbits.TRISH0 = 0; 
    TRISFbits.TRISF0 = 0; 

    //Set all LED outputs as zero
    RedLight = 0;
    BlueLight = 0;
    GreenLight = 0;
    LED1 = 0;
    LED2 = 0;
    HeadLamp = 0;
    MainBeam = 0;
    BrakeLight = 0;
    LeftLamp = 0;
    RightLamp = 0;
  
    // Button RF2 and RF3 Initialisation
    TRISFbits.TRISF2=1; 
    ANSELFbits.ANSELF2=0;
    TRISFbits.TRISF3=1; 
    ANSELFbits.ANSELF3=0; 
}


void LightOn(void){
    RedLight = 1;
    BlueLight = 1;
    GreenLight = 1;
}

void LightOff(void){
    RedLight = 0;
    BlueLight = 0;
    GreenLight = 0;
}


void LightsToggle(void){
    for(char i=0; i <4;i++){
        LED1 = !LED1;
        LED2 = !LED2;
        BrakeLight = !BrakeLight;
        __delay_ms(500);
    }
    __delay_ms(500);
}

