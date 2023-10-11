/*
 * Author:
 * Description: Main file for the project
 */

#include "../include/main.h"

void print_text(const char *str)
{
    printf("%s", str);
}

void check_walls()
{
    if (ir_sensor_read(FRONT))
    {
        // TODO: stop motors
    }
    if (ir_sensor_read(LEFT))
    {
        // TODO: turn right
    }
    if (ir_sensor_read(RIGHT))
    {
        // TODO: tuen left
    }
}

int main()
{
    stdio_init_all();

    while (true)
    {
        print_text("Hello, world!\n");
        check_wallS(); // constantly check walls
        sleep_ms(100);
    }
}
