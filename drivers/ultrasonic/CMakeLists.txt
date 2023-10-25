add_executable(ultrasonic
        ultrasonic.c
        )

# pull in common dependencies
# The current ultrasonic can only be communicate via i2c
target_link_libraries(ultrasonic pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(ultrasonic)

# add url via pico_set_program_url
example_auto_set_url(ultrasonic)

# enable usb output, enable uart output
pico_enable_stdio_usb(ultrasonic 1)