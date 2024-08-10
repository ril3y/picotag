#include "sd_card.h"
#include "ff.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "card.h"
#include "hw_config.h"  // Include your hardware config
#include <stdint.h>

static FATFS fs;    // File system object
static FIL fil;     // File object
static bool mounted = false;

// Initialize the SD card SPI pins
sd_spi_pins_t sd_spi_pins = {
    .mosi_pin = SD_SPI_MOSI_PIN,
    .miso_pin = SD_SPI_MISO_PIN,
    .sck_pin = SD_SPI_SCK_PIN,
    .cs_pin = SD_SPI_CS_PIN
};


FRESULT  sdcard_init(void) {
    // Set up the SPI interface with the defined GPIO pins
    spi_init(spi_default, 1000 * 1000); // Initialize SPI at 1 MHz (adjust speed as needed)
    
    gpio_set_function(sd_spi_pins.mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(sd_spi_pins.miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(sd_spi_pins.sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(sd_spi_pins.cs_pin, GPIO_FUNC_SPI);

    // Ensure the CS pin is set to high initially
    gpio_put(sd_spi_pins.cs_pin, 1);
    gpio_set_dir(sd_spi_pins.cs_pin, GPIO_OUT);
        return FR_OK; // Or appropriate FRESULT value

}


FRESULT sdcard_mount(void) {
    FRESULT fr;
    if (mounted) {
        return FR_OK; // Already mounted
    }

    fr = f_mount(&fs, "", 1);  // Mount the default drive
    if (fr == FR_OK) {
        mounted = true;
        printf("SD card mounted successfully.\n");
    } else {
        printf("Failed to mount SD card: %d\n", fr);
    }
    return fr;
}

FRESULT sdcard_unmount(void) {
    FRESULT fr;
    if (!mounted) {
        return FR_OK; // Already unmounted
    }

    fr = f_unmount("");
    if (fr == FR_OK) {
        mounted = false;
        printf("SD card unmounted successfully.\n");
    } else {
        printf("Failed to unmount SD card: %d\n", fr);
    }
    return fr;
}

FRESULT sdcard_list_files(void) {
    FRESULT fr;
    DIR dir;
    FILINFO fno;

    fr = f_opendir(&dir, "/");  // Open the root directory
    if (fr == FR_OK) {
        while (1) {
            fr = f_readdir(&dir, &fno);  // Read a directory item
            if (fr != FR_OK || fno.fname[0] == 0) break;  // Break on error or end of dir
            if (fno.fattrib & AM_DIR) {
                printf("  <DIR>  %s\n", fno.fname);
            } else {
                printf("  %s\n", fno.fname);
            }
        }
        f_closedir(&dir);
    } else {
        printf("Failed to open directory: %d\n", fr);
    }
    return fr;
}

FRESULT sdcard_read_file(const char *filename) {
    FRESULT fr;
    UINT br;
    char buffer[128];

    fr = f_open(&fil, filename, FA_READ);
    if (fr == FR_OK) {
        printf("Reading file: %s\n", filename);
        do {
            fr = f_read(&fil, buffer, sizeof(buffer)-1, &br);  // Read a chunk of data from the file
            if (fr != FR_OK) {
                printf("Failed to read file: %d\n", fr);
                break;
            }
            buffer[br] = 0;  // Null-terminate the string
            printf("%s", buffer);
        } while (br > 0);
        f_close(&fil);
    } else {
        printf("Failed to open file: %d\n", fr);
    }
    return fr;
}

FRESULT sdcard_write_file(const char *filename, const char *data) {
    FRESULT fr;
    UINT bw;

    fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr == FR_OK) {
        printf("Writing to file: %s\n", filename);
        fr = f_write(&fil, data, strlen(data), &bw);
        if (fr == FR_OK) {
            printf("Wrote %u bytes to file.\n", bw);
        } else {
            printf("Failed to write to file: %d\n", fr);
        }
        f_close(&fil);
    } else {
        printf("Failed to open file: %d\n", fr);
    }
    return fr;
}
