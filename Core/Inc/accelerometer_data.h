/*
 * File: accelerometer_data.h
 * Purpose: Header file for accelerometer_data.c 
 * 
 * Created on: Sep 20, 2021
 * Author: Sam Donnelly
 */

#include "stm32f4xx_hal.h"

#ifndef ACCEL_DATA
#define ACCEL_DATA

// Initiate accelerometer
void MPU6050_Init(void);

// Read accelerometer data 
float* MPU6050_read(float CONST, uint8_t ADDR, uint8_t REG, float data_corr[]);

#endif
