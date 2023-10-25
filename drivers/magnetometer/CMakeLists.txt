add_executable(magnetometer
        magnetometer.c
        )

# pull in common dependencies
# The current magnetometer can only be communicate via i2c
target_link_libraries(magnetometer pico_stdlib hardware_i2c)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(magnetometer)

# add url via pico_set_program_url
example_auto_set_url(magnetometer)

# enable usb output, enable uart output
pico_enable_stdio_usb(magnetometer 1)