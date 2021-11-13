/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "i2c-lcd.h"
#include "accelerometer_data.h"

// Libraries
#include <stdlib.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Structures ----------------------------------------------------------------------------------

// Primary state machine structure
typedef const struct BSDA {
	void (*output_func)(void);
	unsigned long time_delay;
	unsigned long next_state[NUM_OPTIONS];
} BSDA;

// State Machines

// Primary state machine
BSDA FSM[NUM_STATES] = {
		{&startup,        S0_DELAY, {normal_state,         normal_state         }},
		{&normal,         S1_DELAY, {normal_state,         accel_cal_prep_state }},
		{&accel_cal_prep, S2_DELAY, {accel_cal_prep_state, accel_cal_state      }},
		{&accel_cal,      S3_DELAY, {normal_state,         normal_state         }}
};


// Global variables ----------------------------------------------------------------------------

uint8_t flag = 0;     // Interrupt flag
unsigned long state;  // Current state
unsigned long input;  // For various inputs

float accel_data[3];
float *accel_data_p;
float accel_corr[3];
float *accel_corr_p;
float gyro_data[3];
float *gyro_data_p;
float gyro_corr[3];
float *gyro_corr_p;

char buf[5];      // Accelerometer data buffer


// Functions -----------------------------------------------------------------------------------

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
	}

	else {
		lcd_send_string("Sensor Not Resp");
		HAL_Delay(5000);
	}
}


void startup(void) {
	// Initialize everything on startup
	lcd_init();
	MPU6050_Init();

	accel_corr_p = MPU6050_read_accel_raw(ACCEL_CONST, MPU6050_ADDR, ACCEL_XOUT_H_REG, accel_corr);
	gyro_corr_p  = MPU6050_read_gyro_raw(GYRO_CONST, MPU6050_ADDR, GYRO_XOUT_H_REG, gyro_corr);

	accel_corr[0] = accel_corr_p[0];
	accel_corr[1] = accel_corr_p[1];
	accel_corr[2] = accel_corr_p[2];
	gyro_corr[0]  = gyro_corr_p[0];
	gyro_corr[1]  = gyro_corr_p[1];
	gyro_corr[2]  = gyro_corr_p[2];

	lcd_send_string("Initialized");
}

void normal(void) {
	// Normal run mode
	uint8_t line_pos;

	accel_data_p = MPU6050_read_accel(ACCEL_CONST, MPU6050_ADDR, ACCEL_XOUT_H_REG, accel_data, 
		accel_corr);
	gyro_data_p  = MPU6050_read_gyro(GYRO_CONST, MPU6050_ADDR, GYRO_XOUT_H_REG, gyro_data, 
		gyro_corr);

	accel_data[0] = accel_data_p[0];
	accel_data[1] = accel_data_p[1];
	accel_data[2] = accel_data_p[2];
	gyro_data[0]  = gyro_data_p[0];
	gyro_data[1]  = gyro_data_p[1];
	gyro_data[2]  = gyro_data_p[2];

	// Print on the screen
	lcd_send_cmd(0x80|0x00);
	lcd_send_string("Ax=       ");
	sprintf(buf, "%.2f", (double)accel_data[0]);
	line_pos = (accel_data[0] >= 0) ? (0x04):(0x03);
	lcd_send_cmd(0x80|line_pos);
	lcd_send_string(buf);
	lcd_send_string("g ");

	lcd_send_cmd(0x80|0x40);
	lcd_send_string("Ay=       ");
	sprintf(buf, "%.2f", (double)accel_data[1]);
	line_pos = (accel_data[1] >= 0) ? (0x44):(0x43);
	lcd_send_cmd(0x80|line_pos);
	lcd_send_string(buf);
	lcd_send_string("g ");

	lcd_send_cmd(0x80|0x14);
	lcd_send_string("Az=       ");
	sprintf(buf, "%.2f", (double)accel_data[2]);
	line_pos = (accel_data[2] >= 0) ? (0x18):(0x17);
	lcd_send_cmd(0x80|line_pos);
	lcd_send_string(buf);
	lcd_send_string("g ");

	lcd_send_cmd(0x80|0x0A);
	lcd_send_string("Gx=       ");
	sprintf(buf, "%.2f", (double)gyro_data[0]);
	line_pos = (gyro_data[0] >= 0) ? (0x0E):(0x0D);
	lcd_send_cmd(0x80|line_pos);
	lcd_send_string(buf);

	lcd_send_cmd(0x80|0x4A);
	lcd_send_string("Gy=       ");
	sprintf(buf, "%.2f", (double)gyro_data[1]);
	line_pos = (gyro_data[1] >= 0) ? (0x4E):(0x4D);
	lcd_send_cmd(0x80|line_pos);
	lcd_send_string(buf);

	lcd_send_cmd(0x80|0x1E);
	lcd_send_string("Gz=       ");
	sprintf(buf, "%.2f", (double)gyro_data[2]);
	line_pos = (gyro_data[2] >= 0) ? (0x22):(0x21);
	lcd_send_cmd(0x80|line_pos);
	lcd_send_string(buf);

	lcd_send_cmd(0x80|0x54);
	lcd_send_string("                    ");
}

void accel_cal_prep(void) {
	// Prepare to calibrate the accelerometer
	flag = 0;

	lcd_clear();

	lcd_send_cmd(0x80|0x00);
	lcd_send_string("Position the");
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("accelerometer to its");
	lcd_send_cmd(0x80|0x14);
	lcd_send_string("zero position");
}

void accel_cal(void) {
	// Calibrate the accelerometer
	flag = 0;

	lcd_clear();

	accel_corr_p = MPU6050_read_accel_raw(ACCEL_CONST, MPU6050_ADDR, ACCEL_XOUT_H_REG, accel_corr);
	gyro_corr_p  = MPU6050_read_gyro_raw(GYRO_CONST, MPU6050_ADDR, GYRO_XOUT_H_REG, gyro_corr);

	accel_corr[0] = accel_corr_p[0];
	accel_corr[1] = accel_corr_p[1];
	accel_corr[2] = accel_corr_p[2];
	gyro_corr[0]  = gyro_corr_p[0];
	gyro_corr[1]  = gyro_corr_p[1];
	gyro_corr[2]  = gyro_corr_p[2];

	lcd_send_cmd(0x80|0x00);
	lcd_send_string("Calibration Complete");
}


// Interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	flag = 1;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	state = startup_state;   // Initialize to state 0

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  // lcd_init();
  // MPU6050_Init();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  // HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, sizeof(msg)/sizeof(msg[0]));

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  (FSM[state].output_func)();
	  HAL_Delay(FSM[state].time_delay);
	  state = FSM[state].next_state[flag];

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
