#include <stdbool.h>

#include "hardware/spi.h"
#include "sd_card.h"
#include "ff.h"

// Define your SPI interface and pin configuration here
static spi_t spi = {
    .hw_inst = spi0,  // Use spi0 or spi1 depending on your setup
    .miso_gpio = 16,  // GPIO pin for MISO
    .mosi_gpio = 19,  // GPIO pin for MOSI
    .sck_gpio = 18,   // GPIO pin for SCK
    .baud_rate = 10000000, // Set SPI clock speed (10MHz here)
    .DMA_IRQ_num = DMA_IRQ_0, // Set DMA IRQ if using DMA
    .use_exclusive_DMA_IRQ_handler = false,
    .no_miso_gpio_pull_up = false,
};

// Define the SPI interface for the SD card
static sd_spi_if_t sd_spi = {
    .spi = &spi,
    .ss_gpio = 11,  // GPIO pin for CS (chip select)
};

// Define the SD card instance
static sd_card_t sd_card = {
    .type = SD_IF_SPI,
    .spi_if_p = &sd_spi,
};

// Return the number of SD cards
size_t sd_get_num() {
    return 1;
}

// Return the pointer to the SD card instance by index
sd_card_t *sd_get_by_num(size_t num) {
    if (num != 0) {
        return NULL;
    }
    return &sd_card;
}
