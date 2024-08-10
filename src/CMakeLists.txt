cmake_minimum_required(VERSION 3.12)

# Set the PICO_SDK_PATH to the local submodule path
set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../pico-sdk")

# Include build functions from Pico SDK
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set the compiler with full path
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc CACHE STRING "" FORCE)
set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++ CACHE STRING "" FORCE)
set(CMAKE_ASM_COMPILER /usr/bin/arm-none-eabi-gcc CACHE STRING "" FORCE)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(blueTag C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()



# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
    blueTag.c
    card.c
    hw_config.c  # Add your hardware config file
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${PICO_SDK_PATH}/src/rp2_common/hardware_spi/include  # This line includes the correct path to spi.h
    ${CMAKE_CURRENT_LIST_DIR}/lib/no-OS-FatFs/include 
    ${CMAKE_CURRENT_LIST_DIR}/lib/no-OS-FatFs/src/sd_driver
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Add the subdirectory for the no-OS-FatFs library
add_subdirectory(lib/no-OS-FatFs/src)



# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico  # Link the FatFS library
    hardware_spi
    hardware_dma
    hardware_pio
    hardware_rtc
    cmsis_core
)


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
