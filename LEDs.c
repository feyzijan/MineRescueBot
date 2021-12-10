#include <xc.h>
#include "LEDs.h"

void LEDsInit(void){
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
}


void LightToggle(void){
    RedLight = !RedLight;
    BlueLight = !BlueLight;
    GreenLight = !GreenLight;
}

void LEDtest(void){
    for(char i=0; i <4;i++){
        RedLight = !RedLight;
        BlueLight = !BlueLight;
        GreenLight = !GreenLight;
        LED1 = !LED1;
        LED2 = !LED2;
        HeadLamp = !HeadLamp;
        MainBeam = !MainBeam;
        BrakeLight = !BrakeLight;
        LeftLamp = !LeftLamp;
        RightLamp = !RightLamp;
        __delay_ms(250);
    }
}


