// /*
//  * accelerometer_data.h
//  *
//  *  Created on: Sep 20, 2021
//  *      Author: Sam Donnelly
//  */

#include "stm32f4xx_hal.h"

#ifndef ACCEL_DATA
#define ACCEL_DATA

// Read accelerometer raw data
float* MPU6050_read_accel_raw(float accel_con, uint8_t MPU_ADDR, uint8_t ACCEL_REG, 
 float accel_corr[]);

// Read accelerometer calibrated data
float* MPU6050_read_accel(float accel_con, uint8_t MPU_ADDR, uint8_t ACCEL_REG, 
 float accel_data[], float accel_corr[]);

// Read gyroscope raw data
float* MPU6050_read_gyro_raw(float gyro_con, uint8_t MPU_ADDR, uint8_t GYRO_REG, 
 float gyro_corr[]);

// Read gyroscope calibrated data
float* MPU6050_read_gyro(float gyro_con, uint8_t MPU_ADDR, uint8_t GYRO_REG, 
 float gyro_data[], float gyro_corr[]);

#endif
