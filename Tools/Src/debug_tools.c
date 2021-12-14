/*
 * File: debug_tools.c
 * Purpose: Code to help debugging
 * 
 * Created On: December 13, 2021
 * Author: Sam Donnelly 
 * 
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
#include "debug_tools.h"
#include "stm32f4xx_hal.h"


/* 
 * ---------------------------------------------------------------------------------------------
 * Variables
 * ---------------------------------------------------------------------------------------------
 */

extern UART_HandleTypeDef huart2;



/* 
 * ---------------------------------------------------------------------------------------------
 * Functions 
 * ---------------------------------------------------------------------------------------------
 */

// Send information over uart 
void send_uart(char *string) 
{
    uint8_t len = strlen(string);
    // Transmit in blocking mode
    HAL_UART_Transmit(&huart2, (uint8_t*)string, len, 2000);
}
