/**
 * @file i2c-lcd.h
 * 
 * @author Sam Donnelly
 * 
 * @brief Header file for i2c-lcd.c 
 * 
 * @details 
 * 
 * @version 0.1
 * @date 2022-01-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef LCD_I2C
#define LCD_I2C


//======================================================================================

#include "includes.h"

//======================================================================================


//======================================================================================
// 

// Macros 
#define SLAVE_ADDRESS_LCD 0x4E  // LCD address - change this according to your setup

//======================================================================================


//======================================================================================
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

//======================================================================================


#endif
