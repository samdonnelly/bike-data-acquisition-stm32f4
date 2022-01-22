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
// Macros

#define ACCEL_SCALAR 16384
#define GYRO_SCALAR  131

#define MEM_SIZE 1
#define NUM_BYTES 6
#define TIMEOUT 1000

//======================================================================================


//======================================================================================
// Enums

typedef enum {
    MPU6050_ADDR      = 0xD0,   // MPU6050 ID
    SMPLRT_DIV_REG    = 0x19,   // Sample rate divider
    GYRO_CONFIG_REG   = 0x1B,   // Gyroscope configuration
    ACCEL_CONFIG_REG  = 0x1C,   // Accelerometer configuration
    ACCEL_XOUT_H_REG  = 0x3B,   // Accelerometer measurements
    TEMP_OUT_H_REG    = 0x41,   // Temperature measurement
    GYRO_XOUT_H_REG   = 0x43,   // Gyroscope measurements
    PWR_MGMT_1_REG    = 0x6B,   // Power management 1
    WHO_AM_I_REG      = 0x75    // Who am I - verify device identity
} register_and_device_addresses_t;

//======================================================================================


//======================================================================================
// Function Prototypes 

/**
 * @brief Initiate accelerometer
 * 
 */
uint8_t MPU6050_Init(void);


/**
 * @brief Read accelerometer data 
 * 
 * @param trackers 
 * @param ADDR 
 * @param REG 
 * @param data_corr 
 */
void MPU6050_read(
    bda_trackers_t *trackers);

//======================================================================================


#endif
