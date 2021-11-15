/*
 * File: main_functions.h
 * Purpose: Header file for main_functions.c 
 *
 * Created on: November 13, 2021
 * Author: Sam Donnelly
 */

// Libraries 


// Header Files 
#include "main.h"
#include "stm32f4xx_hal.h"

#ifndef MAIN_FUNCTIONS
#define MAIN_FUNCTIONS

// Function Prototypes 
void main_function(void);

// Normal run mode
void normal(void);

// Print to LCD 
void print_data(uint8_t pos1, uint8_t pos2, uint8_t pos3, char str[], uint8_t index, float val,
    int data[]);

// Prepare to calibrate the accelerometer
void accel_cal_prep(void);

// Calibrate the accelerometer
void accel_cal(void);


#endif   // MAIN_FUNCTIONS

