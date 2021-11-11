// /*
//  * accelerometer_data.c
//  *
//  *  Created on: Sep 20, 2021
//  *      Author: Sam Donnelly
//  */

// #include "accelerometer_data.h"
// extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly


// float* MPU6050_read_accel_raw(float accel_con, uint8_t MPU_ADDR, uint8_t ACCEL_REG, float accel_corr[]) {
// 	uint8_t data[6];

// 	// Read 6 bytes of data starting from ACCEL_XOUT_H register
// 	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, ACCEL_REG, 1, data, 6, 1000);

// 	accel_corr[0] = ((int16_t)(data[0] << 8 | data[1]))/accel_con;
// 	accel_corr[1] = ((int16_t)(data[2] << 8 | data[3]))/accel_con;
// 	accel_corr[2] = ((int16_t)(data[4] << 8 | data[5]))/accel_con;

// 	return accel_corr;
// }

// float* MPU6050_read_accel(float accel_con, uint8_t MPU_ADDR, uint8_t ACCEL_REG, float accel_data[], float accel_corr[]) {
// 	uint8_t data[6];

// 	// Read 6 bytes of data starting from ACCEL_XOUT_H register
// 	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, ACCEL_REG, 1, data, 6, 1000);

// 	accel_data[0] = ((int16_t)(data[0] << 8 | data[1]))/accel_con - accel_corr[0];
// 	accel_data[1] = ((int16_t)(data[2] << 8 | data[3]))/accel_con - accel_corr[1];
// 	accel_data[2] = ((int16_t)(data[4] << 8 | data[5]))/accel_con - accel_corr[2];

// 	return accel_data;
// }

// float* MPU6050_read_gyro_raw(float gyro_con, uint8_t MPU_ADDR, uint8_t GYRO_REG, float gyro_corr[]) {
// 	uint8_t data[6];

// 	// Read 6 bytes of data starting from ACCEL_XOUT_H register
// 	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, GYRO_REG, 1, data, 6, 1000);

// 	gyro_corr[0] = ((int16_t)(data[0] << 8 | data[1]))/gyro_con;
// 	gyro_corr[1] = ((int16_t)(data[2] << 8 | data[3]))/gyro_con;
// 	gyro_corr[2] = ((int16_t)(data[4] << 8 | data[5]))/gyro_con;

// 	return gyro_corr;
// }

// float* MPU6050_read_gyro(float gyro_con, uint8_t MPU_ADDR, uint8_t GYRO_REG, float gyro_data[], float gyro_corr[]) {
// 	uint8_t data[6];

// 	// Read 6 bytes of data starting from ACCEL_XOUT_H register
// 	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, GYRO_REG, 1, data, 6, 1000);

// 	gyro_data[0] = ((int16_t)(data[0] << 8 | data[1]))/gyro_con - gyro_corr[0];
// 	gyro_data[1] = ((int16_t)(data[2] << 8 | data[3]))/gyro_con - gyro_corr[1];
// 	gyro_data[2] = ((int16_t)(data[4] << 8 | data[5]))/gyro_con - gyro_corr[2];

// 	return gyro_data;
// }


