/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

// Initialize everything on startup
void startup(void);

// Normal run mode
void normal(void);

// Prepare to calibrate the accelerometer
void accel_cal_prep(void);

// Calibrate the accelerometer
void accel_cal(void);

// Initiate accelerometer
void MPU6050_Init(void);

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

// initialize lcd
void lcd_init (void);

// send command to the lcd
void lcd_send_cmd (char cmd);

// send data to the lcd
void lcd_send_data (char data);

// send string to the lcd
void lcd_send_string (char *str);

// Clear display
void lcd_clear (void);  

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

// State machine states
#define startup_state         0
#define normal_state          1
#define accel_cal_prep_state  2
#define accel_cal_state       3

// Accelerometer hardware registers
#define MPU6050_ADDR       0xD0      // MPU6050 ID

#define SMPLRT_DIV_REG     0x19      // Sample rate divider
#define GYRO_CONFIG_REG    0x1B      // Gyroscope configuration
#define ACCEL_CONFIG_REG   0x1C      // Accelerometer configuration
#define ACCEL_XOUT_H_REG   0x3B      // Accelerometer measurements
#define TEMP_OUT_H_REG     0x41      // Temperature measurement
#define GYRO_XOUT_H_REG    0x43      // Gyroscope measurements
#define PWR_MGMT_1_REG     0x6B      // Power management 1
#define WHO_AM_I_REG       0x75      // Who am I - verify device identity

// Constants
#define NUM_OPTIONS  2
#define NUM_STATES   4
#define S0_DELAY     3000
#define S1_DELAY     500
#define S2_DELAY     1000
#define S3_DELAY     2000

#define ACCEL_CONST  16384.0
#define GYRO_CONST   131.0

// LCD address - change this according to your setup
#define SLAVE_ADDRESS_LCD 0x4E

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
