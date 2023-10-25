add_executable(encoder
        encoder.c
        )

# pull in common dependencies
target_link_libraries(encoder pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(encoder)

# add url via pico_set_program_url
example_auto_set_url(encoder)

# enable usb output, enable uart output
pico_enable_stdio_usb(encoder 1)