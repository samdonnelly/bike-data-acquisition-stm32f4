/**
 * @file main_functions.h
 * 
 * @author Sam Donnelly
 * 
 * @brief Header file for main_functions.c 
 * 
 * @details 
 * 
 * @version 0.1
 * @date 2022-01-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef MAIN_FUNCTIONS
#define MAIN_FUNCTIONS


//======================================================================================
// Libraries and Header Files

// #include "main.h"
// #include "stm32f4xx_hal.h"

#include "includes.h"

//======================================================================================


//======================================================================================
// Function Prototypes 
 
// Psudo main function 
void main_function(void);

// Normal run mode
void normal(void);

// Print to LCD 
void print_data(
    uint8_t pos1, 
    uint8_t pos2, 
    uint8_t pos3, 
    char str[], 
    uint8_t index, 
    float val,
    int data[]);

// Prepare to calibrate the accelerometer
void accel_cal_prep(void);

// Calibrate the accelerometer
void accel_cal(void);

// Send information over UART
void send_uart(char *string);

//======================================================================================


#endif   // MAIN_FUNCTIONS
