/**
 * @file main_functions.c
 * 
 * @author Sam Donnelly
 * 
 * @brief Main source file created to avoid using main.c as much as possible due 
 *        to all the generated code by STM32CubeMX making it hard to read.  
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
// Libraries and Header Files

#include "main_functions.h"

//======================================================================================


//======================================================================================
// List of functions to point to 

static bda_state_functions_t state_table[NUM_STATES] = {&startup_state,
                                                       &idle_state,
                                                       &mode_set_state,
                                                       &system_check_state,
                                                       &pre_recording_state,
                                                       &recording_state,
                                                       &post_recording_state,
                                                       &fault_state,
                                                       &low_power_mode_state};

//======================================================================================


//======================================================================================
// Variables

// External global variables 
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart2;

// Interrupt flag
uint8_t flag;

// Current state
unsigned long state;  

// Accelerometer data handlers 
int accel_data_value[9];
float *accel_data_p;
float accel_corr[3];
float *accel_corr_p;
int gyro_data_value[9];
float *gyro_data_p;
float gyro_corr[3];
float *gyro_corr_p;
float zero_corr[3] = {0.0, 0.0, 0.0};

// Accelerometer data buffer
char buf[6];

// Position of text on LCD display 
uint8_t line_pos;

// String to print to the LCD display 
char Ax_string[] = "Ax=       ";
char Ay_string[] = "Ay=       ";
char Az_string[] = "Az=       ";
char Gx_string[] = "Gx=       ";
char Gy_string[] = "Gy=       ";
char Gz_string[] = "Gz=       ";

//======================================================================================


//======================================================================================
// Psudo Main Function 

void main_function(void) 
{
    // Local variables 
    static bda_trackers_t marin_apline_trail_7;
    static bda_states_t next_state;
    static uint16_t input; // temporary placeholder variable 
     
    flag = 0;
    state = accel_cal_state;

    // Initialize components
    // Init data structure here 
    lcd_init();
    MPU6050_Init();
    lcd_send_string("Initialized");
    HAL_Delay(1500);

    // SD tests ------------------------------------

    // Initialize SD card 
    bool sd_card_1_status = sd_card_init(); 

    if (sd_card_1_status)
    {
        lcd_send_cmd(0x80|0x00);
	    lcd_send_string("SD card mounted");
        lcd_send_cmd(0x80|0x40);
	    lcd_send_string("successfully");
        HAL_Delay(3000);
        lcd_clear();
    }
    else 
    {
        lcd_send_cmd(0x80|0x00);
	    lcd_send_string("Error mounting SD");
        lcd_send_cmd(0x80|0x40);
	    lcd_send_string("card");
        HAL_Delay(3000);
        lcd_clear();

        // TODO If there is an error then enter an error state 
    }

    // Get available card space
    uint16_t sd_card_total_space;
    uint16_t sd_card_free_space;

    sd_card_total_space = sd_card_space(TOTAL_SPACE);
    sd_card_free_space  = sd_card_space(FREE_SPACE);

    lcd_send_cmd(0x80|0x00);
    lcd_send_string("SD card total size:");
    lcd_send_cmd(0x80|0x40);
    // sprintf(buf, "%" PRIu16 "", sd_card_total_space);
    sprintf(buf, "%d", (char)sd_card_total_space);
    lcd_send_string(buf);
        // (char)((sd_card_total_space / 1000)),
        // (char)((sd_card_total_space / 100) % 10),
        // (char)((sd_card_total_space / 10) % 10),
        // (char)((sd_card_total_space) % 10));
    HAL_Delay(3000);
    lcd_clear();

    lcd_send_cmd(0x80|0x00);
    lcd_send_string("SD card free space:");
    lcd_send_cmd(0x80|0x40);
    sprintf(buf, "%" PRIu16 "", sd_card_free_space);
    lcd_send_string(buf);
    HAL_Delay(3000);
    lcd_clear();

    // sd_card(CREATE);

    // sd_card(UPDATE);

    // sd_card(REMOVE);
    
    // Main loop 
    while(1) 
    {
        // Make copy of pointer to function 
        next_state = marin_apline_trail_7.bda_state;

        // Inputs to system 
        // Read users inputs here 

        // State Machine - shell (will be filled out later)
        switch (next_state)
        {
        case STARTUP_STATE:
            if (input)
            {
                next_state = IDLE_STATE;
            }

            break;

        case IDLE_STATE:
            if (input)
            {
                next_state = MODE_SET_STATE;
            }
            
            break;

        case MODE_SET_STATE:
            if (input)
            {
                next_state = SYSTEM_CHECK_STATE;
            }
            
            break;

        case SYSTEM_CHECK_STATE:
            if (input)
            {
                next_state = PRE_RECORDING_STATE;
            }
            
            break;

        case PRE_RECORDING_STATE:
            if (input)
            {
                next_state = RECORDING_STATE;
            }
            
            break;

        case RECORDING_STATE:
            if (input)
            {
                next_state = POST_RECORDING_STATE;
            }
            
            break;

        case POST_RECORDING_STATE:
            if (input)
            {
                next_state = FAULT_STATE;
            }
            
            break;

        case FAULT_STATE:
            if (input)
            {
                next_state = LOW_POWER_MODE_STATE;
            }
            
            break;

        case LOW_POWER_MODE_STATE:
            if (input)
            {
                next_state = STARTUP_STATE;
            }
            
            break;

        default:
            break;
        }

        // Outputs 
        marin_apline_trail_7.bda_state = next_state;

    }
}

//======================================================================================


//======================================================================================
// Interrupts

// External Interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	flag = 1;
}

//======================================================================================

//======================================================================================
// Normal run mode
void normal(void) {

    // Read linear acceleration data (x, y, z) - in units of gravity (g's)
    accel_data_p = MPU6050_read(ACCEL_CONST, MPU6050_ADDR, ACCEL_XOUT_H_REG, accel_corr);	

    // Parse linear acceleration data (x, y, z)
	accel_data_value[0] = abs((int)accel_data_p[0]);
	accel_data_value[1] = abs((int)(10*accel_data_p[0])  % 10);
    accel_data_value[2] = abs((int)(100*accel_data_p[0]) % 10);

	accel_data_value[3] = abs((int)accel_data_p[1]);
	accel_data_value[4] = abs((int)(10*accel_data_p[1])  % 10);
    accel_data_value[5] = abs((int)(100*accel_data_p[1]) % 10);

	accel_data_value[6] = abs((int)accel_data_p[2]);
	accel_data_value[7] = abs((int)(10*accel_data_p[2])  % 10);
    accel_data_value[8] = abs((int)(100*accel_data_p[2]) % 10);

    // Read angular acceleration data (alpha, beta, gamma) - in units of deg/s
    gyro_data_p  = MPU6050_read(GYRO_CONST, MPU6050_ADDR, GYRO_XOUT_H_REG, gyro_corr);

     // Parse angular acceleration data (alpha, beta, gamma)
    gyro_data_value[0] = abs((int)gyro_data_p[0]);
	gyro_data_value[1] = abs((int)(10*gyro_data_p[0])  % 10);
    gyro_data_value[2] = abs((int)(100*gyro_data_p[0]) % 10);

	gyro_data_value[3] = abs((int)gyro_data_p[1]);
	gyro_data_value[4] = abs((int)(10*gyro_data_p[1])  % 10);
    gyro_data_value[5] = abs((int)(100*gyro_data_p[1]) % 10);

	gyro_data_value[6] = abs((int)gyro_data_p[2]);
	gyro_data_value[7] = abs((int)(10*gyro_data_p[2])  % 10);
    gyro_data_value[8] = abs((int)(100*gyro_data_p[2]) % 10);

	// Print parsed data to the screen
    // Ax
    print_data(0x00, 0x04, 0x03, Ax_string, 0, accel_data_p[0], accel_data_value);

    // Ay
    print_data(0x40, 0x44, 0x43, Ay_string, 3, accel_data_p[1], accel_data_value);

    // Az
    print_data(0x14, 0x18, 0x17, Az_string, 6, accel_data_p[2], accel_data_value);

    // Gx
    print_data(0x0A, 0x0E, 0x0D, Gx_string, 0, gyro_data_p[0], gyro_data_value);

    // Gy
    print_data(0x4A, 0x4E, 0x4D, Gy_string, 3, gyro_data_p[1], gyro_data_value);

    // Gz
    print_data(0x1E, 0x22, 0x21, Gz_string, 6, gyro_data_p[2], gyro_data_value);

    // To ensure any additional text that was printed to the screen is cleared
	lcd_send_cmd(0x80|0x54);
	lcd_send_string("                    ");
}

//======================================================================================


//======================================================================================
// Print parsed data to the LCD display 
void print_data(
    uint8_t pos1, 
    uint8_t pos2, 
    uint8_t pos3, 
    char str[], 
    uint8_t index, 
    float val,
    int data[]) 
{

    lcd_send_cmd(0x80|pos1);
	lcd_send_string(str);
    if (val >= 0) {
        line_pos = pos2;
        sprintf(buf, "%d.%d%d", data[index], data[index+1], data[index+2]);
    }
    else {
        line_pos = pos3;
        sprintf(buf, "-%d.%d%d", data[index], data[index+1], data[index+2]);
    }
	lcd_send_cmd(0x80|line_pos);
	lcd_send_string(buf);
}

//======================================================================================


//======================================================================================
// Prepare to calibrate the accelerometer
void accel_cal_prep(void) 
{
    if (flag == 1) {
        lcd_clear();
        flag = 0;
    }

	lcd_send_cmd(0x80|0x00);
	lcd_send_string("Position the");
	lcd_send_cmd(0x80|0x40);
	lcd_send_string("accelerometer to its");
	lcd_send_cmd(0x80|0x14);
	lcd_send_string("zero position");
}

//======================================================================================


//======================================================================================
// Calibrate the accelerometer
void accel_cal(void) 
{
	lcd_clear();
    flag = 0;
	
    accel_corr_p = MPU6050_read(ACCEL_CONST, MPU6050_ADDR, ACCEL_XOUT_H_REG, zero_corr);

    accel_corr[0] = accel_corr_p[0];
	accel_corr[1] = accel_corr_p[1];
	accel_corr[2] = accel_corr_p[2];

	gyro_corr_p  = MPU6050_read(GYRO_CONST, MPU6050_ADDR, GYRO_XOUT_H_REG, zero_corr);

	gyro_corr[0]  = gyro_corr_p[0];
	gyro_corr[1]  = gyro_corr_p[1];
	gyro_corr[2]  = gyro_corr_p[2];

	lcd_send_cmd(0x80|0x00);
	lcd_send_string("Calibration Complete");
}

//======================================================================================


//======================================================================================
// Startup State

void startup_state(void)
{
    // 
}

//======================================================================================


//======================================================================================
// Startup State

void idle_state(void)
{
    // 
}

//======================================================================================


//======================================================================================
// Startup State

void mode_set_state(void)
{
    // 
}

//======================================================================================


//======================================================================================
// Startup State

void system_check_state(void)
{
    // 
}

//======================================================================================


//======================================================================================
// Startup State

void pre_recording_state(void)
{
    // 
}

//======================================================================================


//======================================================================================
// Startup State

void recording_state(void)
{
    // 
}

//======================================================================================


//======================================================================================
// Startup State

void post_recording_state(void)
{
    // 
}

//======================================================================================


//======================================================================================
// Startup State

void fault_state(void)
{
    // 
}

//======================================================================================


//======================================================================================
// Startup State

void low_power_mode_state(void)
{
    // 
}

//======================================================================================

