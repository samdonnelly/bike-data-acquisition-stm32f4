/**
 * @file accelerometer_data.c
 * 
 * @author Sam Donnelly
 * 
 * @brief Functions that interact with the systems accelerometer
 * 
 * @details 
 * 
 * @version 0.1
 * @date 2022-01-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//======================================================================================
// Includes

#include "accelerometer_data.h"

//======================================================================================


//======================================================================================
// Variables 

extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

uint8_t data[6];
float accel_data[3];

//======================================================================================


//======================================================================================
// Accelerometer initialization

void MPU6050_Init(void) {

	uint8_t check;
	uint8_t Data;

	// check ID WHO_AM_I
	HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1, 1000);

	// If the device is present
	if (check == 104) {  // 104 -> 0x68
		// Power management register 0x6B we should write all 0's to wake the sensor up
		Data = 0;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1, 1000);

		// Set DATA RATE of 1kHz by writing SMPLRT_DIV register
		Data = 0x07;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000);

		// Set accelerometer configuration in ACCEL_CONFIG register
		// XA_ST = 0, YA_ST = 0, ZA_ST = 0, FS_SEL = 0 -> +/- 2g
		Data = 0x00;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);

		// Set gyroscope configuration in GYRO_CONFIG register
		// XG_ST = 0, YG_ST = 0, ZG_ST = 0, FS_SEL = 0 -> +/- 250 deg/s
		Data = 0x00;
		HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);

		// lcd_send_string("Initialized");
	}

	else {
		lcd_send_string("Sensor Not Resp");
		HAL_Delay(5000);
	}
}

//======================================================================================


//======================================================================================
// Read data from accelerometer

void MPU6050_read(bda_trackers_t *trackers) 
{
	// accel_data[0] = ((int16_t)(data[0] << 8 | data[1]))/CONST - data_corr[0];
	// accel_data[1] = ((int16_t)(data[2] << 8 | data[3]))/CONST - data_corr[1];
	// accel_data[2] = ((int16_t)(data[4] << 8 | data[5]))/CONST - data_corr[2];

	// trackers->x_accel = ((int16_t)(data[0] << 8 | data[1]))/ACCEL_SCALAR - data_corr[0];
	// trackers->y_accel = ((int16_t)(data[2] << 8 | data[3]))/ACCEL_SCALAR - data_corr[1];
	// trackers->z_accel = ((int16_t)(data[4] << 8 | data[5]))/ACCEL_SCALAR - data_corr[2];

	// trackers->x_rotation = ((int16_t)(data[0] << 8 | data[1]))/GYRO_SCALAR  - data_corr[0];
	// trackers->y_rotation = ((int16_t)(data[2] << 8 | data[3]))/GYRO_SCALAR  - data_corr[1];
	// trackers->z_rotation = ((int16_t)(data[4] << 8 | data[5]))/GYRO_SCALAR  - data_corr[2];

	// Read 6 bytes of data starting from ACCEL_XOUT_H register
	// Read linear acceleration data (x, y, z) - in units of gravity (g's)
	HAL_I2C_Mem_Read(
		&hi2c1, 
		MPU6050_ADDR, 
		ACCEL_XOUT_H_REG, 
		(uint16_t)MEM_SIZE, 
		data, 
		(uint16_t)NUM_BYTES, 
		(uint32_t)TIMEOUT);

	trackers->x_accel = 
			(int16_t)(((((int16_t)data[0] << 8) | ((int16_t)data[1])) / ACCEL_SCALAR) * 100);

	trackers->y_accel = 
			(int16_t)(((((int16_t)data[2] << 8) | ((int16_t)data[3])) / ACCEL_SCALAR) * 100);

	trackers->z_accel = 
			(int16_t)(((((int16_t)data[4] << 8) | ((int16_t)data[5])) / ACCEL_SCALAR) * 100);


	// Read angular acceleration data (alpha, beta, gamma) - in units of deg/s
	HAL_I2C_Mem_Read(
		&hi2c1, 
		MPU6050_ADDR, 
		GYRO_XOUT_H_REG, 
		(uint16_t)MEM_SIZE, 
		data, 
		(uint16_t)NUM_BYTES, 
		(uint32_t)TIMEOUT);

	trackers->x_rotation = 
			(int16_t)(((((int16_t)data[0] << 8) | ((int16_t)data[1])) / GYRO_SCALAR) * 100);

	trackers->y_rotation = 
			(int16_t)(((((int16_t)data[2] << 8) | ((int16_t)data[3])) / GYRO_SCALAR) * 100);

	trackers->z_rotation = 
			(int16_t)(((((int16_t)data[4] << 8) | ((int16_t)data[5])) / GYRO_SCALAR) * 100);
}

//======================================================================================
