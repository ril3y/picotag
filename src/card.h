#ifndef SDCARD_H
#define SDCARD_H

#include "sd_card.h"
#include "ff.h"

#include <stdint.h>

// Define the GPIO pins used for SPI communication with the SD card
#define SD_SPI_MOSI_PIN   19   // GPIO pin number for MOSI
#define SD_SPI_MISO_PIN   16   // GPIO pin number for MISO
#define SD_SPI_SCK_PIN    18   // GPIO pin number for SCK
#define SD_SPI_CS_PIN     11   // GPIO pin number for CS

// You can also define other relevant configurations here
typedef struct {
    uint8_t mosi_pin;
    uint8_t miso_pin;
    uint8_t sck_pin;
    uint8_t cs_pin;
} sd_spi_pins_t;

extern sd_spi_pins_t sd_spi_pins;


// Function to initialize the SD card
FRESULT sdcard_init(void);

// Function to mount the filesystem
FRESULT sdcard_mount(void);

// Function to unmount the filesystem
FRESULT sdcard_unmount(void);

// Function to list files in the root directory
FRESULT sdcard_list_files(void);

// Function to read a file
FRESULT sdcard_read_file(const char *filename);

// Function to write to a file
FRESULT sdcard_write_file(const char *filename, const char *data);

#endif // SDCARD_H
