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

typedef struct spi_vars spi_vars;

typedef enum {
    INIT,
    CREATE,
    UPDATE,
    REMOVE
} SD_card_functions;


/* 
 * ---------------------------------------------------------------------------------------------
 * Function prototypes
 * ---------------------------------------------------------------------------------------------
 */

// Main SD card function 
void sd_card(uint8_t func);

// Initialize SD card 
void sd_card_init(spi_vars *card);

// Create file on SD card 
void sd_card_create_file(void);

// Update an existing file 
void sd_card_update_file(void);

// Remove file
void sd_card_remove_file(void);

// Determine size of buffer up to which it is filled
int bufsize(char *buf);

// Clear buffer 
void bufclear(spi_vars *card_buffer);



/* 
 * ---------------------------------------------------------------------------------------------
 * End include guards
 * ---------------------------------------------------------------------------------------------
 */

#endif  // SD_CARD_SPI
