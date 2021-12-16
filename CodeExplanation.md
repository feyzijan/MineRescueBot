# Final Project: Code Description

## Overview of the ".c/.h" files

**main.c**:   Sets up initialisation functions and initialises motor structure, calls calibration procedures before going into main operating loop

**color.c/.h**:   Contains functions to configure the color click module and interrupt, contains further procedures to calbrate color the interrupt threshold and read and decide between colours

**interrupts.c/.h**   Functions to initialise interrupts sources and a high priority interrupt service routine for the color click interrupt and a timer

**i2c.c/.h**   Set up of the I2C serial communication interface in order to RGBC values from the TCS3471 color click module

**timers.c/.h**   Set up of the Timer0 source and a function to read the current value of the timer

**dc_motor.c/.h**   Define motor structure, configuration of dc motor, functions for specific basic moves e.g. turn left 45 and a calibration routine depending on the surface

**CardMoves.c/.h**  Contains functions to execute specfic motor moves depending on the colour of the card, also contains the a function return to the starting point

**LEDs.c/.h**   Configures buttons on the clicker and LEDs on the buggy and clicker board



## Main Operation
1. First the initialisation functions for the color clicker, LEDs, buttons and DC motor are called

2. Calibration of the color clicker TCS3471 interrupt threshold values

- Hold blue card up to front of buggy with a few millimeters gap while perpendicular to the floor
- press the left button (RF3)
- Leave blue card in front of buggy for at least a second
- Remove blue card and press button and wait for at least one second
- Add black card approximately 7 cm away from the front of the buggy and press the button again, wait for one second
- Remove the black card
- Place buggy at the start point of the tracking course and press left button again to end calibration 

3. Calibration of the DC motor turning accuracy

- FEYZI ADD INSTRUCTIONS
- FEYZI ADD INSTRUCTIONS
- FEYZI ADD INSTRUCTIONS
- FEYZI ADD INSTRUCTIONS

4. Calibrate how far to reverse when meeting a yellow or pink card

- FEYZI ADD INSTRUCTIONS
- FEYZI ADD INSTRUCTIONS
- FEYZI ADD INSTRUCTIONS
- FEYZI ADD INSTRUCTIONS

5. The process is then started via a button click
6. The buggy continues in a straight line until the clear channel of the color click is exceeding causing an interrupt flag to occur
7. The buggy is called to stop and then decide the colour of the card in front of it via **decide_color()** function
8. Depending on the colour detected a specific move is called and the buggy executes this move and continues again in a straight line
9. Additionally, the specific move and time between move is stored in the memory which is crucial in order to allow the buggy to return home



## Distinction of Colours
The **decide_color()** function is responsible for executing the color descision once the buggy is infront of the card

### Reading 1: REFLECTED LIGHT OFF CARD FROM LED + AMBIENT LIGHT + REFLECTED LIGHT OFF CARD FROM AMBIENT LIGHT + CROSS TALK LED LIGHT
-Read the RGBC values with the RGB LED on (white light) and with ambient light

REFLECTED LIGHT OFF CARD FROM LED + AMBIENT LIGHT + REFLECTED LIGHT OFF CARD FROM AMBIENT LIGHT + CROSS TALK LED LIGHT
### Reading 2: AMBIENT LIGHT + REFLECTED LIGHT OFF CARD FROM AMBIENT LIGHT 
-The RGB LED is turned off and the RGB values from the TCS3471 sensor is read again (ambient is still present)

### Corrected Readings: Reading 1 - Reading 2 - CROSS TALK LED LIGHT
-Correct the readings by removing external light sources

-These corrected readings will give accurate RGBC magnitudes purely on the light that is reflected off the card from the RGB LED

-*Note: the cross talk LED light readings for each channel are predetermined constants stored in 'color.h'*
### Normalise Readings
- Take each RGB colour channel corrected reading for the card in front and divide by the corrected clear channel value
- These will be made into integers in the interest of efficiency (i.e. percentage values)
- The normalised percentages have been measured and predetermined at home and plotted graphically to see how each color channel percentage differs from others at distances of about 1-5 cm between the buggy and the cards

### Compare and distinguish colours

AJAY INSERT IMAGE HERE

- The above graph is used to distinguish between common colours
- blue & green, red & orange, pink & yellow, light blue & white are first separted into categories using if statements depending on the range of the normalised red values
- Then each common color is distinguished based on the other normalised color channels with distinguishable differences
- A value from 1-9 is returned by the **decide_color()** which corresponds to a specific colour

## Calibration Procedures


## Journey Memory


## Interrupt Procedure


## Motor Moves


