/*
 * File: sd-card-spi.c
 * Purpose: Provide functions to read and write data from an SD card 
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "sd-card-spi.h"
#include "fatfs_sd.h"


/* 
 * ---------------------------------------------------------------------------------------------
 * Variables
 * ---------------------------------------------------------------------------------------------
 */

typedef struct spi-vars {
    //
    FATFS fs;           // file system 
    FIL fil;            // file 
    FRESULT fresult;    // to store the result 
    char buffer[1024];  // to store data 

    UINT br, bw;        // file read/write count 

    // Capacity related variables 
    FATFS *pfs;
    DWORD fre_clust;
    uint32_t total;
    uint32_t free_space;

} spi-vars;




/* 
 * ---------------------------------------------------------------------------------------------
 * Functions 
 * ---------------------------------------------------------------------------------------------
 */

// Determine size of buffer up to which it is filled
int bufsize (char *buf) {
    int i = 0;
    
    while (*buf++ != '\0') { 
        i++; 
    }
    
    return i; 
}

// Clear buffer 
void bufclear(void) {
    for (int i=0; i < 1024; i++) {
        buffer[i] = '\0';
    }
}

// Send information over uart 
void send_uart(char *string) {
    uint8_t len = strlen(string);
    // Transmit in blocking mode
    HAL_UART_Transmit(&huart2, (uint8_t*)string, len, 2000);
}


// Initialize SD Card
void sd_card_init(void) {
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
}


// Create file on SD card 
void sd_card_create_file(void) {
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
}


// Update an existing file - choose the second file ----------------------------
// Update an existing file 
void sd_card_update_file(void) {
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
}


// Remove files
void sd_card_remove_file(void) {
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
}

