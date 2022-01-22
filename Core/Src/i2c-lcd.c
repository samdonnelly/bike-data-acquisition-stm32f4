/**
 * @file i2c-lcd.c
 * 
 * @author Sam Donnelly
 * 
 * @brief Functions that interact with the systems LCD display
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

#include "i2c-lcd.h"

//======================================================================================


//======================================================================================
// Variables 

extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

//======================================================================================


//======================================================================================
// Functions 

// Initialize the screen 
uint8_t lcd_init (void) 
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

    // display initialization

	// Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	lcd_send_cmd(0x28); 
	HAL_Delay(1);

	// Display on/off control --> D=0,C=0, B=0  ---> display off
	lcd_send_cmd(0x08); 
	HAL_Delay(1);

	// clear display
	lcd_send_cmd(0x01);  
	HAL_Delay(1);

	// Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	lcd_send_cmd(0x06); 
	HAL_Delay(1);

	// Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	lcd_send_cmd(0x0C); 

	// Determine fault condition for LCD screen 
	return FALSE;
}

// LCD send command 
void lcd_send_cmd (char cmd) 
{
  	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data) 
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_clear (void) 
{
	lcd_send_cmd (0x00);
	for (int i=0; i<100; i++) 
	{
		lcd_send_data (' ');
	}
}

void lcd_send_string (char *str) 
{
	while (*str) 
	{
		lcd_send_data(*str++);
	}
}

//======================================================================================
