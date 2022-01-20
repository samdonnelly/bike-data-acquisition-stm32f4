/**
 * @file accelerometer_data.h
 * 
 * @author Sam Donnelly
 * 
 * @brief Header file for accelerometer_data.c 
 * 
 * @details 
 * 
 * @version 0.1
 * @date 2022-01-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef ACCEL_DATA
#define ACCEL_DATA


//======================================================================================
// Libraries 

#include "includes.h"

//======================================================================================


//======================================================================================
// Function Prototypes 

// Initiate accelerometer
void MPU6050_Init(void);

// Read accelerometer data 
float* MPU6050_read(
    float CONST, 
    uint8_t ADDR, 
    uint8_t REG, 
    float data_corr[]);

//======================================================================================


#endif
