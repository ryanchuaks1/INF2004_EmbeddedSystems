/*
 * Author: Ryan Chua
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
        printf("Front\n");
        // TODO: stop motors
    }
    if (ir_sensor_read(LEFT))
    {
        printf("Left\n");
        // TODO: turn right
    }
    if (ir_sensor_read(RIGHT))
    {
        printf("Right\n");
        // TODO: turn left
    }
}

void test_rtos(void *pvParameters)
{
    while (1)
    {
        printf("Hello, world!\n");
        vTaskDelay(1000);
    }
}

void test_barcode(void *pvParameters)
{
    while (1)
    {
    }
    // check_if_wall();
}

int main()
{
    stdio_init_all();
    barcode_init();

    xTaskCreate(test_barcode, "test_barcode", 1024, NULL, 1, NULL);
    vTaskStartScheduler();

    while (1)
    {
    };
}
