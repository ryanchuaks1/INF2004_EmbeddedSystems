/*
 * Author: Rene Low
 * Description: TODO: wall detection
 */

#include "../include/infrared.h"
#define MEASURE_MS 100

/**
 * infrared_task()
 * --------
 * Purpose: Task Used to run the infrared
 * Arguments: Parameters from the task
 * Return: None
 */
void infrared_task(void* params)
{
    //Initialise Left sensor pin
    struct Car* car = (struct Car*)params;
    printf("Hello\n");
    struct Sensor_Data* left_sensor = (struct Sensor_Data*)malloc(sizeof(struct Sensor_Data));
    sensor_data_init(left_sensor);
    left_sensor->pin = LEFT_IR_SENSOR_PIN;
    ir_sensor_init(left_sensor->pin);

    //Initialise Right sensor pin
    struct Sensor_Data* right_sensor = (struct Sensor_Data*)malloc(sizeof(struct Sensor_Data));
    sensor_data_init(right_sensor);
    right_sensor->pin = RIGHT_IR_SENSOR_PIN;
    ir_sensor_init(right_sensor->pin);
    uint16_t duration_ms = 10000;

    while(true){
        
        //Wait for a message from PID to start sensing
        xMessageBufferReceive(*(car->components[INFRARED]->buffer), &duration_ms, sizeof(duration_ms), portMAX_DELAY);

        //Sensing Functions
        printf("IR_Sensor measurement started!\n");
        ir_sensor_enable(left_sensor);
        ir_sensor_enable(right_sensor);
        printf("Finised enabling sensor...\n");
        add_alarm_in_ms(duration_ms, (alarm_callback_t)ir_sensor_disable, left_sensor, false);
        add_alarm_in_ms(duration_ms, (alarm_callback_t)ir_sensor_disable, right_sensor, false);


        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

/**
 * sensor_data_init()
 * --------
 * Purpose: Sets all the sensor pin to default values
 * Arguments: the Sensor
 * Return: None
 */
void sensor_data_init(struct Sensor_Data* sensor){
    sensor->pin = 0;

    //Stores the number of times the sensor senses white
    sensor->status[0] = 0;

    //Stores the number of times the sensor senses black
    sensor->status[1] = 0;
}

/**
 * sensor_measure()
 * --------
 * Purpose: Get and increment the count of what the pin is sensing, black or white
 * Arguments: Repeating Timer
 * Return: None
 */
void sensor_measure(struct repeating_timer *t){
    struct Sensor_Data* sensor = (struct Sensor_Data*)(t->user_data);

    //Increment on Black if Sense Black, vice vera for White
    sensor->status[(uint8_t)gpio_get(sensor->pin)]++;
}

/**
 * ir_sensor_init()
 * --------
 * Purpose: initialise the pins for IR sensing
 * Arguments: Pin number
 * Return: None
 */
void ir_sensor_init(uint8_t pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
}

/**
 * ir_sensor_enable()
 * --------
 * Purpose: Add repeating timer to keep sensing for black or white
 * Arguments: The IR Sensor (Left or Right)
 * Return: None
 */
void ir_sensor_enable(struct Sensor_Data* sensor){
    sensor->status[gpio_get(sensor->pin)]++;
    add_repeating_timer_ms(-MEASURE_MS, (repeating_timer_callback_t)sensor_measure, (void *)sensor, &(sensor->timer));
}

/**
 * ir_sensor_disable()
 * --------
 * Purpose: Disable the IR Sensor to stop sensing
 * Arguments: The alarm ID and the Parameters from the Timer
 * Return: None
 */
int64_t ir_sensor_disable(alarm_id_t id, void* params){
    struct Sensor_Data* sensor = (struct Sensor_Data*)params;
    printf("Disabling sensor...\n");
    bool result = cancel_repeating_timer(&(sensor->timer));
    if(result){
        printf("Result of 1 minute of measurement. Black: %d, White: %d\n", sensor->status[1], sensor->status[0]);
        
    }
    else{
        printf("Failed to stop timer\n");
    }

    return 0;
}