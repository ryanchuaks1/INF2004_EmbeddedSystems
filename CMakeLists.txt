cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Add FreeRTOS Kernel
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
# include(FreeRTOS_Kernel_import.cmake)

project(pico_car_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)

# Initialize the SDK
pico_sdk_init()

# Include directories for your project
include_directories(include)  # Add this line

# Add your source files to the project
add_executable(pico_car_project
    src/main.c
    src/grid.c
    src/linkedlist.c
    src/mapping.c
    src/node.c
    src/pathfinding.c
    src/sorting.c
    src/barcode.c
    src/infrared.c
    src/motor.c
    src/ultrasonic.c
    src/user_interface.c
    src/magnetometer.c
    src/wheel_encoder.c
    src/interrupt.c
)

# Link the Pico SDK libraries to your project
target_link_libraries(pico_car_project
    pico_stdlib
    hardware_pwm
    hardware_adc
    hardware_gpio
    hardware_irq
    hardware_i2c
    
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http

    FreeRTOS-Kernel-Heap4
)

# Add your FreeRTOSConfig.h file to the include directories
target_include_directories(pico_car_project PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)


# Set FreeRTOS compile definitions
target_compile_definitions(pico_car_project PRIVATE
    WIFI_SSID=\"${WIFI_SSID}\"
    WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
)

# makefsdata python script
execute_process(COMMAND
    py makefsdata.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

# Enable USB serial output (optional)
pico_enable_stdio_usb(pico_car_project 1)
pico_enable_stdio_uart(pico_car_project 0)

# Create a UF2 image for flashing
pico_add_extra_outputs(pico_car_project)
