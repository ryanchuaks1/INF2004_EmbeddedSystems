/*
 * Author: Ryan Chua
 * Description: Main file for the project
 */

#include "../include/main.h"
#include <stdlib.h>

void print_text(const char *str)
{
    printf("%s", str);
}

void check_walls()
{
    printf("%s\n", gpio_get(LEFT_IR_SENSOR_PIN) ? "BLACK" : "WHITE");
    // if (ir_sensor_read(FRONT))
    // {
    //     //printf("Front\n");
    //     // TODO: stop motors
    // }
    // if (ir_sensor_read(LEFT))
    // {
    //     //printf("Left\n");
    //     // TODO: turn right
    // }
    // if (ir_sensor_read(RIGHT))
    // {
    //     //printf("Right\n");
    //     // TODO: turn left
    // }
}

void test_rtos()
{
    while (true)
    {
        printf("Hello, world!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        // ir_sensor_init();
        // check_walls();
    }
}

void ir_wall_task(void* params){
    struct Sensor_Data* left_sensor = (struct Sensor_Data*)malloc(sizeof(struct Sensor_Data));
    sensor_data_init(left_sensor);
    left_sensor->pin = LEFT_IR_SENSOR_PIN;
    ir_sensor_init(left_sensor->pin);

    struct Sensor_Data* right_sensor = (struct Sensor_Data*)malloc(sizeof(struct Sensor_Data));
    sensor_data_init(right_sensor);
    right_sensor->pin = RIGHT_IR_SENSOR_PIN;
    ir_sensor_init(right_sensor->pin);

    bool task_started = false;

    while(true){

        if(!task_started){
            task_started = true;
            vTaskDelay(pdMS_TO_TICKS(5000));
            printf("IR_Sensor measurement started!\n");
            ir_sensor_enable(left_sensor);
            ir_sensor_enable(right_sensor);
            printf("Finised enabling sensor...\n");
            add_alarm_in_ms(10000, (alarm_callback_t)ir_sensor_disable, left_sensor, false);
            add_alarm_in_ms(10000, (alarm_callback_t)ir_sensor_disable, right_sensor, false);

            // left_sensor.status[0] = 0;
            // left_sensor.status[1] = 0;
        }

    }

}

int main()
{
    stdio_init_all();
    //xTaskCreate(test_rtos, "test_rtos", 1024, NULL, 1, NULL);
    xTaskCreate(ir_wall_task, "ir_wall_task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
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
