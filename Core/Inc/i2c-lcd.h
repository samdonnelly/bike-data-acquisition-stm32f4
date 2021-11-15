/*
 * File: i2c-lcd.h
 * Purpose: Header file for i2c-lcd.c 
 * 
 * Created on: Sep 20, 2021
 * Author: External 
 */

#include "stm32f4xx_hal.h"

#ifndef LCD_I2C
#define LCD_I2C

// Macros 
#define SLAVE_ADDRESS_LCD 0x4E // change this according to your setup

// Function Prototypes

// initialize lcd
void lcd_init(void);

// send command to the lcd
void lcd_send_cmd(char cmd);

// send data to the lcd
void lcd_send_data(char data);

// send string to the lcd
void lcd_send_string(char *str);

// Clear the screen 
void lcd_clear(void);  

#endif
