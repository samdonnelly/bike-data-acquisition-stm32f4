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
 * Libraries and Header Files
 * ---------------------------------------------------------------------------------------------
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
 * Function prototypes
 * ---------------------------------------------------------------------------------------------
 */

// Determine size of buffer up to which it is filled
int bufsize(char *buf);

// Clear buffer 
void bufclear(void);

// Send information over UART
void send_uart(char *string);

// Initialize SD card 
void sd_card_init(void);

// Create file on SD card 
void sd_card_create_file(void);

// Update an existing file 
void sd_card_update_file(void);

// Remove file
void sd_card_remove_file(void);



/* 
 * ---------------------------------------------------------------------------------------------
 * End include guards
 * ---------------------------------------------------------------------------------------------
 */

#endif  // SD_CARD_SPI
