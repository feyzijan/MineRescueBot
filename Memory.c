#include <xc.h>
#include "Memory.h"



//Initialise variables
int timeList[30];
char time_index = 0;

// Array of functions to call
card_func funcPtrList[30];
char func_index = 0;


void add_function_ptr (card_func func){
    funcPtrList[func_index++] = func;
    if (func_index >= 30) lost_flag = 1;
}


void add_timing(unsigned int timing){
    timeList[time_index++] = timing;
}

// Make this return the 16bit
unsigned int get_timing(void){
    return timeList[--time_index];
}


card_func get_function_ptr(void){
    return funcPtrList[--func_index];
}

