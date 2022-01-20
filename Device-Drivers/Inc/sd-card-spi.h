/**
 * @file sd-card-spi.h
 * 
 * @author Sam Donnelly 
 * 
 * @brief 
 * 
 * @version 0.1
 * @date 2022-01-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SD_CARD_SPI
#define SD_CARD_SPI


//======================================================================================
// Includes

#include "includes.h"

//======================================================================================


//======================================================================================
// Variables 

typedef struct sd_card_file_system sd_card_file_system;


typedef enum {
    TOTAL_SPACE,
    FREE_SPACE
} SD_card_functions;

// TODO Define SD card number in an enum and pass that as a parameter to the functions 
// to know what sd card instance to use

//======================================================================================


//======================================================================================
// Function prototypes

// Initialize SD card 
bool sd_card_init(void);

// SD card total and free space 
uint16_t sd_card_space(uint8_t space_parameter);

// Create file on SD card 
void sd_card_create_file(void);

// Update an existing file 
void sd_card_update_file(void);

// Remove file
void sd_card_remove_file(void);

// Determine size of buffer up to which it is filled
int bufsize(char *buf);

// Clear buffer 
void bufclear(sd_card_file_system *card_buffer);

//======================================================================================


#endif  // SD_CARD_SPI
