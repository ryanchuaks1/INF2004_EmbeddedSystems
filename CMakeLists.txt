cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_car_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add your source files to the project
add_executable(pico_car_project
    src/main.c
    src/barcode_recognition.c
    src/infrared.c
    src/mapping.c
    src/movement_control.c
    src/obstacle_detection.c
    src/user_interface.c
)

# Link the Pico SDK libraries to your project
target_link_libraries(pico_car_project
    pico_stdlib
    hardware_adc
    hardware_pwm
)

# Enable USB serial output (optional)
pico_enable_stdio_usb(pico_car_project 1)
pico_enable_stdio_uart(pico_car_project 0)

# Create a UF2 image for flashing
pico_add_extra_outputs(pico_car_project)
