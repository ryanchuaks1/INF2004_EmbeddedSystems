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

void test_rtos()
{
    while (true)
    {
        printf("Hello, world!\n");
        vTaskDelay(1000);
    }
} 

int main()
{
    stdio_init_all();
    xTaskCreate(test_rtos, "test_rtos", 1024, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
    // ir_sensor_init();

    // while (true)
    // {
    //     print_text("Hello, world!\n");
    //     check_walls(); // constantly check walls
    //     sleep_ms(100);
    // }
}
