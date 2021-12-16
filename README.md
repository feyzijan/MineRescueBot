# Course project - Mine navigation search and rescue

## Overview of the ".c/.h" files

**main.c**:   Sets up initialisation functions and initialises motor structure, calls calibration procedures before going into main operating loop

**color.c/.h**:   Contains functions to configure the color click module and interrupt, contains further procedures to calbrate color the interrupt threshold and read and decide between colours

**interrupts.c/.h**   Functions to initialise interrupts sources and a high priority interrupt service routine for the color click interrupt and a timer

**i2c.c/.h**   Set up of the I2C serial communication interface in order to RGBC values from the TCS3471 color click module

**timers.c/.h**   Set up of the Timer0 source and a function to read the current value of the timer

**dc_motor.c/.h**   Define motor structure, configuration of dc motor, functions for specific basic moves e.g. turn left 45 and a calibration routine depending on the surface

**CardMoves.c/.h**  Contains functions to execute specfic motor moves depending on the colour of the card, also contains the a function return to the starting point

**LEDs.c/.h**   Configures buttons on the clicker and LEDs on the buggy and clicker board


## Main Operating Procedure
- Turn on Buggy and go through the Calibrations for Clicker nterrupt threshold, and the two motor calibrations, to ensure desirable performance in different ambient and surface settings (explained below)

- Check that the interrupt thresholds have been calibrated correctly by moving the buggy near up to different colored cards, and seeing if the interrupt is triggeered ( back lights should light up)
     - Press ButtonRF3 to reset the interrupt and try it out for different cards - if interrupts keep contintually triggering have the buggy even closer to the blue card during calibration

- Once you are ready start set the buggy in the starting position and press ButtonRF2
- The buggy will now commence the main operating loop

### Main Loop:
- The buggy moves in a straight line until a clicker interrupt is triggered
- When this happens the buggy stops, records the duration of its just ended journey in memory
- It takes readings of the RGBC channels with Led On, and Off, normalises the readings to percentage values, and decides which color it has seen
- It passes this color value to a function that picks and executes the appropriate move, while storing the complement function(explained below) in memory
- The buggy restarts the timer from 0 and keeps repeating this process until it is told to go back home upon reading the white card, or when an exception is triggered (more below) 
- When told to go home the buggy goes through its memory of stored movement times and functions and executes them successively to go back home

## Interrupt Threshold Calibration

**interrupt_threshold_calibrate** function is used to set the upper and lower interrupt thresholds
Procedure:
 - Lights flash to indicate start of calibration
 
- Hold blue card up to front of buggy with a few millimeters gap while perpendicular to the floor
- press the left button (RF3)
- Leave blue card in front of buggy for at least a second
- Remove blue card and press button and wait for at least one second
- Add black card approximately 7 cm away from the front of the buggy and press the RF3 button again, wait for one second
- Remove the black card
- Place buggy at the start point of the tracking course and press RF3 button again to end calibration 

## Motor Calibrations

**CalibrateTurns** function is used to modify the global variable turning_time to achieve accurate turns
Procedure:
 - Lights flash to indicate start of calibration
 
 - The buggy then performs a 180 turn - User has 4 options
 
 1. Car turns too much -> Hold Button RF2: Decrease turning_time
 
 2. Car turns too little -> Hold Button RF3: Increase turning_time
 
 3. Want to see again? -> Hold Button RF2 and RF3 simultaneously
 
 --- Buttons must be held till respective LED flashes---
 
 4. Hold Button RF2 and RF3 until back lights flash to exit calibration
 
 - Lights flash to indicate end of calibration



**CalibrateReverseSquare** function is used to modify the global variable reverse_time to achieve accurate turns

Procedure:

 - Lights flash to indicate start of calibration
 
 - The buggy then performs a reverse_square move - User has 4 options
 
 1. Car moves too much -> Hold Button RF2: Decrease turning_time
 
 2. Car moves too little -> Hold Button RF3: Increase turning_time
 
 3. Want to see again? -> Hold Button RF2 and RF3 simultaneously
 
  -- Buttons must be held till respective LED flashes --
  
 4. Hold Button RF2 and RF3 until back lights flash to exit calibration
 
 - Lights flash to indicate start of calibration


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

- A graph is used to distinguish between common colours (see excel file)
- blue & green, red & orange, pink & yellow, light blue & white are first separted into categories using if statements depending on the range of the normalised red values
- Then each common color is distinguished based on the other normalised color channels with distinguishable differences
- A value from 1-9 is returned by the **decide_color()** which corresponds to a specific colour

## Memory Operations
The buggy relies on two arrays to return home when needed
 - time_list :  A list of TMR0 readings ( converted from bits to ms)
 - funcPtrList : A list of pointers to functions that take only left and right motor structs as input

When the buggy is told to return home it first performs a 180 turn
Then it loops through the two lists in memory, starting from the latest entry
- First it moves the buggy forward by the amount of time in the time_list
- Then it calls the function in the function list

### Complement Functions:
The complement function is the function that needs to be executed for the buggy, coming from the opposite direciton, needs to perform to undo the move.

These can intiutively be mapped as follows:
- Red and Green are inverse of each other
- Orange and Lightblue are inverse of eachother
- Blue is inverse with itself
- For the yellow and pink card reverse_yellow_move and reverse_pink_move functions
    - These first turn the buggy 90 degrees, then move forward one square

## Color Functions
Each card has a move function assocaited with it in CardMoves.c (pink_move, red_move etc)
These functions are called when that color is read and the color value (1-9) is passed to pick_move()
Before that card function is called the complement is stored in the funciton pointer list
Each color function comprises of instruction to TurnLeft/Right n times, and also to go back one square distance for pink and yellow

## Exceptions
The buggy will normally only try to go back home upon reading the white card. However there are three scenarios where it will attempt to go back home prematurely
All these scenarious make the buggy react as if it saw a white card

**1. A black color is detected when the buggy hits a black wall face forward**
 - This means the buggy has hit the wall and will not be able to continue its motion. 
 - This funcitonality can be turned off (say if you are allowed to correct the buggy) by setting the lower interrupt threshold to low, as the black wall is set to only trigger interrupts and register then

**2. The lost_flag is set due to TMR0 overflow**
 - When the time spent in forward motion without detecting any card exceeds a predetermined value (~67s with current configurations) the buggy will go back home
 - This would be useful in scenarious where the person laying the color cards was unable to place the next card and the buggy just goes on moving forward forever
 - If the white card was taken out (and the black color detection was disabled) this would make the buggy go home some time after hitting the wall
 - If the buggy were directed to go on an infinite drive through the desert this would stop that pleasant drive after some time and make it go home

The timing can be configured by changing the TMR0 prescaler - !! You must change the operation in getTMR0_in_ms() that converts TMR0 bits to miliseconds to be stored in this case!! 


**3. The lost_flag is set due to Function Memory overlow**
- The function pointer and movement time arrays are declared with a maximum size (currently 30)
- If the buggy reads more cards than this memory can hold an exception will be triggered and it will go back home immediately after reading the last card
- This is ideal for scenarious where the white flag is not present and the buggy will go on an infinite loop, won't hit the wall, and won't trigger a TMR0 interrupt because it successfully detects card 
 
 This overflow can be configured by setting the maximum memory size approriately.

## Meeting Challenge Brief Objective
We believe we have met all the objectives of the challenge bried outlined below.

## Performance 
Video of Maze3 Performance
https://imperiallondon-my.sharepoint.com/:v:/g/personal/fce19_ic_ac_uk/ESVhCReK7ClBoqI6i86UBuMB2svlf2nOec0hTsIo9OmWWQ?e=3URy1B
Video of Maze2 Performance
https://imperiallondon-my.sharepoint.com/:v:/g/personal/fce19_ic_ac_uk/ERobOBb5J5ZGrGsr6G4VQeQBHswFRX8Q2XBU6sOURCF48g?e=KN85v6


## Challenge brief

Develop an autonomous robot that can navigate a "mine" using a series of instructions coded in coloured cards and return to its starting position.  Your robot must be able to perform the following: 

1. Navigate towards a coloured card and stop before impacting the card
1. Read the card colour
1. Interpret the card colour using a predefined code and perform the navigation instruction
1. When the final card is reached, navigate back to the starting position
1. Handle exceptions and return back to the starting position if final card cannot be found

## "Mine" environment specification

A "mine" is contstructed from black plywood walls 100mm high with some walls having coloured cards located on the sides of the maze to assist with navigation. The following colour code is to be used for navigation:

Colour | Instruction
---------|---------
Red | Turn Right 90
Green | Turn Left 90
Blue | Turn 180
Yellow | Reverse 1 square and turn right 90
Pink | Reverse 1 square and turn left 90
Orange | Turn Right 135
Light blue | Turn Left 135 
White | Finish (return home)
Black | Maze wall colour

Mine courses will vary in difficulty, with the simplest requiring 4 basic moves to navigate. More advanced courses may require 10 or moves to navigate. The mines may have features such as dead ends but colour cards will always direct you to the end of the maze. Once the end of the maze has been reached, you must return to the starting position. An example course to navigate is shown below. You do not know in advance which colours will be in the course or how many.

![Navi Diagram](gifs/maze.gif)
