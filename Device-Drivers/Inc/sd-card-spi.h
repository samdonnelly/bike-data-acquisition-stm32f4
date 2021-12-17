/*
 * File: sd-card-spi.h
 * Purpose: header file to sd-card-spi.c  
 * 
 * Created On: December 9, 2021
 * Author: Sam Donnelly 
 * 
 */


/* 
 * ---------------------------------------------------------------------------------------------
 * Include guards
 * ---------------------------------------------------------------------------------------------
 */

#ifndef SD_CARD_SPI
#define SD_CARD_SPI


/* 
 * ---------------------------------------------------------------------------------------------
 * Libraries and Header Files
 * ---------------------------------------------------------------------------------------------
 */

#include <stdint.h>

#include "integer.h"


/* 
 * ---------------------------------------------------------------------------------------------
 * Variables 
 * ---------------------------------------------------------------------------------------------
 */

typedef struct sd_card_file_system sd_card_file_system;


typedef enum {
    TOTAL_SPACE,
    FREE_SPACE
} SD_card_functions;

// TODO Define SD card number in an enum and pass that as a parameter to the functions 
// to know what sd card instance to use


/* 
 * ---------------------------------------------------------------------------------------------
 * Function prototypes
 * ---------------------------------------------------------------------------------------------
 */

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



/* 
 * ---------------------------------------------------------------------------------------------
 * End include guards
 * ---------------------------------------------------------------------------------------------
 */

#endif  // SD_CARD_SPI
