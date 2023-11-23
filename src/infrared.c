/*
 * Author: Rene Low
 * Description: TODO: wall detection
 */

#include "../include/infrared.h"
// #define MEASURE_MS 100

void ir_sensor_init(){
    gpio_init(LEFT_IR_SENSOR_PIN);
    gpio_init(FRONT_IR_SENSOR_PIN);
    gpio_set_dir(LEFT_IR_SENSOR_PIN, GPIO_IN);
    gpio_set_dir(FRONT_IR_SENSOR_PIN, GPIO_IN);

    gpio_set_irq_enabled_with_callback(LEFT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_RISE, true, &interrupt_callback);
    gpio_set_irq_enabled_with_callback(FRONT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_RISE, true, &interrupt_callback);
}

// void infrared_task(void* params)
// {
//     struct Car* car = (struct Car*)params;
//     printf("Hello\n");
//     struct Sensor_Data* left_sensor = (struct Sensor_Data*)malloc(sizeof(struct Sensor_Data));
//     sensor_data_init(left_sensor);
//     left_sensor->pin = LEFT_IR_SENSOR_PIN;
//     ir_sensor_init(left_sensor->pin);

//     struct Sensor_Data* right_sensor = (struct Sensor_Data*)malloc(sizeof(struct Sensor_Data));
//     sensor_data_init(right_sensor);
//     right_sensor->pin = RIGHT_IR_SENSOR_PIN;
//     ir_sensor_init(right_sensor->pin);
//     uint16_t duration_ms = 10000;

//     while(true){

//         xMessageBufferReceive(*(car->components[INFRARED]->buffer), &duration_ms, sizeof(duration_ms), portMAX_DELAY);
//         printf("IR_Sensor measurement started!\n");
//         ir_sensor_enable(left_sensor);
//         ir_sensor_enable(right_sensor);
//         printf("Finised enabling sensor...\n");
//         add_alarm_in_ms(duration_ms, (alarm_callback_t)ir_sensor_disable, left_sensor, false);
//         add_alarm_in_ms(duration_ms, (alarm_callback_t)ir_sensor_disable, right_sensor, false);


//         vTaskDelay(pdMS_TO_TICKS(1000));

//     }
// }

// void sensor_data_init(struct Sensor_Data* sensor){
//     sensor->pin = 0;
//     sensor->status[0] = 0;
//     sensor->status[1] = 0;
// }

// void sensor_measure(struct repeating_timer *t){
//     struct Sensor_Data* sensor = (struct Sensor_Data*)(t->user_data);
//     // printf("Measure result: %d\n", gpio_get(sensor->pin));
//     // if(gpio_get(sensor->pin)){
//     //     printf("incremnting black...\n");
//     //    sensor->status[1] = sensor->status[0] + 1;
//     // }
//     // else{
//     //     printf("incremnting white...\n");
//     //     sensor->status[0] = sensor->status[0] + 1;
//     // }
//     sensor->status[(uint8_t)gpio_get(sensor->pin)]++;
// }

// void ir_sensor_init(uint8_t pin)
// {
//     gpio_init(pin);
//     gpio_set_dir(pin, GPIO_IN);
// }

// void ir_sensor_enable(struct Sensor_Data* sensor){
//     sensor->status[gpio_get(sensor->pin)]++;
//     add_repeating_timer_ms(-MEASURE_MS, (repeating_timer_callback_t)sensor_measure, (void *)sensor, &(sensor->timer));
// }

// int64_t ir_sensor_disable(alarm_id_t id, void* params){
//     struct Sensor_Data* sensor = (struct Sensor_Data*)params;
//     printf("Disabling sensor...\n");
//     bool result = cancel_repeating_timer(&(sensor->timer));
//     if(result){
//         printf("Result of 1 minute of measurement. Black: %d, White: %d\n", sensor->status[1], sensor->status[0]);
        
//     }
//     else{
//         printf("Failed to stop timer\n");
//     }

//     return 0;
// }