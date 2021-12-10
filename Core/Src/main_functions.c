/*
 * File: main_functions.c
 * Purpose: Main source file created to avoid using main.c as much as possible due 
 *          to all the generated code by STM32CubeMX making it hard to read.  
 * 
 * Created on: Sep 20, 2021
 * Author: External
 */

/* 
 * ---------------------------------------------------------------------------------------------
 * Libraries and Header Files
 * ---------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "main.h"
#include "main_functions.h"
#include "i2c-lcd.h"
#include "accelerometer_data.h"
#include "fatfs.h"
#include "fatfs_sd.h"


/* 
 * ---------------------------------------------------------------------------------------------
 * Variables
 * ---------------------------------------------------------------------------------------------
 */

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

// SPI variables --------------------
FATFS fs;           // file system 
FIL fil;            // file 
FRESULT fresult;    // to store the result 
char buffer[1024];  // to store data 

UINT br, bw;        // file read/write count 

// Capacity related variables 
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;
// ----------------------------------


/* 
 * ---------------------------------------------------------------------------------------------
 * Structures and lists
 * ---------------------------------------------------------------------------------------------
 */

// Primary state machine structure
typedef const struct BSDA {
	void (*output_func)(void);
	unsigned long time_delay;
	unsigned long next_state[NUM_OPTIONS];
} BSDA;

// Primary state machine
BSDA FSM[NUM_STATES] = {
    {&normal,         S1_DELAY, {normal_state,         accel_cal_prep_state }},
    {&accel_cal_prep, S2_DELAY, {accel_cal_prep_state, accel_cal_state      }},
    {&accel_cal,      S3_DELAY, {normal_state,         normal_state         }}
};


/* 
 * ---------------------------------------------------------------------------------------------
 * Psudo Main Function 
 * ---------------------------------------------------------------------------------------------
 */
void main_function(void) {
     
    flag = 0;
    state = accel_cal_state;

    lcd_init();
    MPU6050_Init();
    lcd_send_string("Initialized");
    HAL_Delay(1500);

    // SD Card init -------------------------------------------------
    
    // Mount SD card
    fresult = f_mount(&fs, "", 0);
    if (fresult != FR_OK) {
        send_uart("Error in mounting SD card.\r\n");
    }
    else {
        send_uart("SD card mounted successfully.\r\n");
    }

    // Get capacity of SD card 
    f_getfree("", &fre_clust, &pfs);

    total = (uint32_t)((pfs->n_fatent -2) * pfs->csize * 0.5);
    sprintf(buffer, "SD card total size: \t%lu\r\n", total);
    send_uart(buffer);
    bufclear();
    free_space = (uint32_t)(fre_clust*(pfs->csize)*0.5);
    sprintf(buffer, "SD card free space: \t%lu\r\n", free_space);
    send_uart(buffer);

    // Create our first file on SD card -----------------------------

    // Open file to write/create a file if it doesn't exist 
    fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
    // FA_OPEN_ALWAYS is used for creating the file 
    // FA_READ and FA_WRITE are used for setting the permissions of the file 

    // Writing text then close file 
    fresult = f_puts("This data is from the First FILE\r\n", &fil);
    fresult = f_close(&fil);
    send_uart("File1.txt created and the data is written\r\n");

    // Open file to read
    fresult = f_open(&fil, "file1.txt", FA_READ);

    // Read string from file and close file 
    f_gets(buffer, f_size(&fil), &fil);
    f_close(&fil);
    send_uart(buffer);

    bufclear();

    // Use f_write and f_read to do the same thing as above --------------------

    // Create second file with read write access and open it 
    fresult = f_open(&fil, "file2.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

    // Write text - this will overwite existing data 
    strcpy(buffer, "This is File 2 and it says Hello from S A M\r\n");
    fresult = f_write(&fil, buffer, bufsize(buffer), &bw);  // Takes data from buffer -> file
    // bw is pointer to counter for number of bytes written  
    send_uart("File2.txt created and data is written\r\n");
    f_close(&fil);
    bufclear();

    // Open second file for reading 
    fresult = f_open(&fil, "file2.txt", FA_READ);

    // Read data from file and save in buffer 
    f_read(&fil, buffer, f_size(&fil), &br);
    // sizeof(buffer) is size of data in the file 
    // br is the pointer to the counter variables for number of bytes to read from the file 
    send_uart(buffer);
    f_close(&fil);

    bufclear();


    // Update an existing file - choose the second file ----------------------------

    // Take pointer to file and offset as arguments. The offset is where you want to 
    // start writing in the file. If you want to start writing at the end then you can use 
    // the file size as the offset. 

    // Open the file with write access 
    fresult = f_open(&fil, "file2.txt", FA_OPEN_EXISTING | FA_READ | FA_WRITE);

    // Move to the offset at the end of the file 
    fresult = f_lseek(&fil, f_size(&fil));

    // Write the string to the file 
    fresult = f_puts("This is updated data and it should be in the end\r\n", &fil);
    f_close(&fil);

    bufclear();

    // Open to read the file 
    fresult = f_open(&fil, "file2.txt", FA_READ);

    // Read string from the file 
    f_read(&fil, buffer, f_size(&fil), &br);
    send_uart(buffer);

    f_close(&fil);

    bufclear();


    // Remove a files ----------------------------------------------------------------

    fresult = f_unlink("/file1.txt");
    if (fresult == FR_OK) {
        send_uart("file1.txt removed successfully\r\n");
    }

    fresult = f_unlink("/file2.txt");
    if (fresult == FR_OK) {
        send_uart("file2.txt removed successfully\r\n");
    }

    // Unmount SD card 
    fresult = f_mount(NULL, "/", 1);
    if (fresult == FR_OK) {
        send_uart("SD card unmounted successfully\r\n");
    }


    // Main loop 
    while(1) {

        // Execute state machine 
        (FSM[state].output_func)();
        HAL_Delay(FSM[state].time_delay);
        state = FSM[state].next_state[flag];

    }
}


/* 
 * ---------------------------------------------------------------------------------------------
 * Functions
 * ---------------------------------------------------------------------------------------------
 */

// Determine size of buffer up to which it is filled
int bufsize (char *buf) {
    int i = 0;
    while (*buf++ != '\0') { i++; }
    return i; 
}

// Clear buffer 
void bufclear(void) {
    for (int i=0; i<1024; i++) {
        buffer[i] = '\0';
    }
}

// Send information over uart 
void send_uart(char *string) {
    uint8_t len = strlen(string);
    // Transmit in blocking mode
    HAL_UART_Transmit(&huart2, (uint8_t*)string, len, 2000);
}

// External Interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	flag = 1;
}

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

// Print parsed data to the LCD display 
void print_data(uint8_t pos1, uint8_t pos2, uint8_t pos3, char str[], uint8_t index, float val,
    int data[]) {

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

// Prepare to calibrate the accelerometer
void accel_cal_prep(void) {

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

// Calibrate the accelerometer
void accel_cal(void) {

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
