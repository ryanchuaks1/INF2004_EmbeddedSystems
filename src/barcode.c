/*
 * Author: Ryan Chua
 * Description: TODO: Add description
 */

#include "../include/barcode.h"


/**
 * barcode_task()
 * --------
 * Purpose: VTask for IR Sensor Sensing the Barcode to be ran by freeRtos.
 *          Task runs and read the Barcode
 * Arguments: VTask Parameters
 * Return: None
 */
void barcode_task(void *params)
{
    struct Car* car = (struct Car*)params;
    barcode_init(); // Initialise barcode scanner,

    uint8_t opcode = 10;

    while (true)
    {
        //Wait to receive from the message from the PID controller to measure the barcode
        xMessageBufferReceive
        (
            *(car->components[BARCODE]->buffer),
            (void*)&opcode,
            sizeof(opcode),
            portMAX_DELAY
        );

        printf("the opcode is: %d", opcode);

        while (barcodeFlags.isBarcode)
        {
            uint16_t reading = adc_read();
            printf("Reading: %d\n", reading);

            if (reading > BARCODE_THRESHOLD && !barcodeFlags.isPrevBlackBar)
            {
                //If is Black and the previously bar is not black
                //
                barcodeFlags.isPrevBlackBar = true; // Set to prevBlack = true as currently is black
                int timing = time_us_64() - last_button_press_time; // Measure the timing of the Black Bar
                black_bar_times[bar_index] = timing; // Save the timing
            }
            else if (reading < BARCODE_THRESHOLD && barcodeFlags.isPrevBlackBar)
            {
                //If is white and the previous bar is Black
                //
                barcodeFlags.isPrevBlackBar = false; // Set to prevBlack = false as currently is white
                int timing = time_us_64() - last_button_press_time; // Measure time of the White Bar
                white_bar_times[bar_index] = timing; // Save the timing
                bar_index++; //Increase the Index
            }
            if (white_bar_times[4] != 0)
            {
                // When it reaches the end, decode the barcode
                decode_barcode(black_bar_times, white_bar_times);
                barcodeFlags.isPrevBlackBar = false;
                white_bar_times[4] = 0;
                bar_index = 0;
            }
            if (barcodeFlags.limitter > BARCODE_CHAR_LIMIT)
            {
                // Check if is the ending delimeter, if it is reset all paremeters
                reset_barcode_params();
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // Tell the PID controller that the barcode has been sent
        xMessageBufferSend
        (
            *(car->main_buffer),
            (void*)&opcode,
            sizeof(opcode),
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * reset_barcode_params()
 * --------
 * Purpose: Reset all parameters and flags of the barcode task
 * Arguments: None
 * Return: None
 */
void reset_barcode_params()
{
    barcodeFlags.isPrevBlackBar = false;
    barcodeFlags.isBarcode = false;
    barcodeFlags.count = 0;
    barcodeFlags.limitter = 0;
    last_button_press_time = 0;
    bar_index = 0;
    gpio_set_irq_enabled_with_callback(WALL_SENSOR_PIN, GPIO_IRQ_EDGE_RISE, true, &interrupt_callback); // enable rising edge interrupt
}

/**
 * barcode_init()
 * --------
 * Purpose: Initialises all the pins for the Barcode IR Sensor
 * Arguments: None
 * Return: None
 */
void barcode_init()
{
    printf("initializing barcode\n");
    // Initialize the ADC
    adc_init();
    adc_gpio_init(BARCODE_SENSOR_PIN);
    adc_select_input(BARCODE_ADC_CHANNEL);

    reset_barcode_params();

    gpio_set_irq_enabled_with_callback(WALL_SENSOR_PIN, GPIO_IRQ_EDGE_RISE, true, &interrupt_callback); // enable rising edge interrupt
}

/**
 * alarm_callback()
 * --------
 * Purpose: This alarm_callback tells the PID controller that there is a wall ahead instead of a barcode
 * Arguments: None
 * Return: None
 */
void alarm_callback()
{
    if (barcodeFlags.isBarcode == false)
    {
        uint8_t message = INFRARED;
        xMessageBufferSend(
            *(global_car->main_buffer),
            (void *)&message,
            sizeof(message),
            portMAX_DELAY);
        printf("Wall detected please reverse robot\n");
    }
}

/**
 * check_if_wall()
 * --------
 * Purpose: This functions checks if the Black line detected by the Barcode IR sensor is a Black Wall or a Barcode
 * Arguments: None
 * Return: None
 */
void check_if_wall()
{
    if (time_us_64() - last_button_press_time > DEBOUNCE_DELAY_MS * 1000)
    {
        barcodeFlags.count++;
        last_button_press_time = time_us_64(); // update last button press time
        last_wall_time = time_us_64();         // update last wall time
        add_alarm_in_ms(500, (alarm_callback_t)alarm_callback, NULL, false); //Call the alarm_callback to tell the PID controller that it is a wall

        if (barcodeFlags.count > 1) // When wall is detected
        {
            // Disable interrupt and set flag
            gpio_set_irq_enabled_with_callback(WALL_SENSOR_PIN, GPIO_IRQ_EDGE_RISE, false, &interrupt_callback); // enable rising edge interrupt
            barcodeFlags.isBarcode = true;

            printf("Barcode Detected please reverse robot\n");
        }
    }
}

/**
 * decode_barcode()
 * --------
 * Purpose: Decode the barcode base on the Black and White bar array data from read_barcode
 * Arguments: black_bar_times[], white_bar_times[]
 * Return: None
 */
void decode_barcode(int black_bar_times[], int white_bar_times[])
{
    int dec_black_bar_times[] = {0, 0, 0, 0, 0}; // Array for black bar times
    int dec_white_bar_times[] = {0, 0, 0, 0, 0}; // Array for white bar times
    dec_black_bar_times[0] = (white_bar_times[0] - black_bar_times[0]);
    dec_black_bar_times[1] = (white_bar_times[1] - black_bar_times[1]);
    dec_black_bar_times[2] = (white_bar_times[2] - black_bar_times[2]);
    dec_black_bar_times[3] = (white_bar_times[3] - black_bar_times[3]);
    dec_black_bar_times[4] = (white_bar_times[4] - black_bar_times[4]);

    dec_white_bar_times[0] = (black_bar_times[1] - white_bar_times[0]);
    dec_white_bar_times[1] = (black_bar_times[2] - white_bar_times[1]);
    dec_white_bar_times[2] = (black_bar_times[3] - white_bar_times[2]);
    dec_white_bar_times[3] = (black_bar_times[4] - white_bar_times[3]);

    // printf("Black bar times: %d %d %d %d %d\n", dec_black_bar_times[0], dec_black_bar_times[1], dec_black_bar_times[2], dec_black_bar_times[3], dec_black_bar_times[4]);
    // printf("White bar times: %d %d %d %d\n\n", dec_white_bar_times[0], dec_white_bar_times[1], dec_white_bar_times[2], dec_white_bar_times[3]);

    int max1 = 0;
    int max2 = 0;

    for (int i = 0; i < 5; i++)
    {
        if (dec_black_bar_times[i] > max1)
        {
            max2 = max1;
            max1 = dec_black_bar_times[i];
        }
        else if (dec_black_bar_times[i] > max2)
        {
            max2 = dec_black_bar_times[i];
        }
    }

    // Set the two highest to 1, the rest to 0 for black bars
    for (int i = 0; i < 5; i++)
    {
        if (dec_black_bar_times[i] == max1 || dec_black_bar_times[i] == max2)
        {
            dec_black_bar_times[i] = 1;
        }
        else
        {
            dec_black_bar_times[i] = 0;
        }
    }

    // Find the highest value in white_bar_times
    int max_white = dec_white_bar_times[0];
    for (int i = 1; i < 4; i++)
    {
        if (dec_white_bar_times[i] > max_white)
        {
            max_white = dec_white_bar_times[i];
        }
    }

    // Set the highest to 1, the rest to 0 for white bars
    for (int i = 0; i < 4; i++)
    {
        if (dec_white_bar_times[i] == max_white)
        {
            dec_white_bar_times[i] = 1;
        }
        else
        {
            dec_white_bar_times[i] = 0;
        }
    }
    printf("Black bar times: %d %d %d %d %d\n", dec_black_bar_times[0], dec_black_bar_times[1], dec_black_bar_times[2], dec_black_bar_times[3], dec_black_bar_times[4]);
    printf("White bar times: %d %d %d %d\n\n", dec_white_bar_times[0], dec_white_bar_times[1], dec_white_bar_times[2], dec_white_bar_times[3]);
    barcode_to_char(dec_black_bar_times, dec_white_bar_times);
}

/**
 * barcode_to_char()
 * --------
 * Purpose: Convert the Barcode to Character based on the Black and White bar array data from decode_barcode
 *          Coded the logic using the reference from https://en.m.wikipedia.org/wiki/Code_39 
 * Arguments: black_bar_times[], white_bar_times[]
 * Return: None
 */
void barcode_to_char(int black_bar_times[], int white_bar_times[])
{
    printf("Decoding barcode");

    int result = 0;

    if (black_bar_times[0] && black_bar_times[4])
        result += 1;
    else if (black_bar_times[1] && black_bar_times[4])
        result += 2;
    else if (black_bar_times[0] && black_bar_times[1])
        result += 3;
    else if (black_bar_times[2] && black_bar_times[4])
        result += 4;
    else if (black_bar_times[0] && black_bar_times[2])
        result += 5;
    else if (black_bar_times[1] && black_bar_times[2])
        result += 6;
    else if (black_bar_times[3] && black_bar_times[4])
        result += 7;
    else if (black_bar_times[0] && black_bar_times[3])
        result += 8;
    else if (black_bar_times[1] && black_bar_times[3])
        result += 9;
    else if (black_bar_times[2] && black_bar_times[3])
        result += 10;

    if (white_bar_times[1])
        result += 0;
    else if (white_bar_times[2])
        result += 9;
    else if (white_bar_times[3])
        result += 19;
    else if (white_bar_times[0])
        result += 29;

    printf("Result: %d\n", result);
    decoded_char = code_39_characters[result];
    if (decoded_char == '*')
    {
        barcodeFlags.limitter++;
    }
    printf("Decoded character: %c\n", decoded_char);
}

/**
 * read_barcode()
 * --------
 * Purpose: To read the Barcode. The Thickness of the barcode is measured using Time. 
 *          For Independent Testing, similiar to the code in the barcode_task
 * Arguments: None
 * Return: None
 */
void read_barcode()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (barcodeFlags.isBarcode)
    {
        uint16_t reading = adc_read();

        if (reading > BARCODE_THRESHOLD && !barcodeFlags.isPrevBlackBar)
        {
            //If is Black and the previously bar is not black
            //
            barcodeFlags.isPrevBlackBar = true; // Set to prevBlack = true as currently is black
            int timing = time_us_64() - last_button_press_time; // Mesure the timing of the Black Bar
            black_bar_times[bar_index] = timing; // Save the timing
        }
        else if (reading < BARCODE_THRESHOLD && barcodeFlags.isPrevBlackBar)
        {
            //If is white and the previous bar is Black
            //
            barcodeFlags.isPrevBlackBar = false; // Set to prevBlack = false as currently is white
            int timing = time_us_64() - last_button_press_time; // Measure time of the White Bar
            white_bar_times[bar_index] = timing; // Save the Timing
            bar_index++;
        }
        if (white_bar_times[4] != 0)
        {
            // When it reaches the end, decode the barcode
            decode_barcode(black_bar_times, white_bar_times);
            barcodeFlags.isPrevBlackBar = false;
            white_bar_times[4] = 0;
            bar_index = 0;
        }
        if (barcodeFlags.limitter == 2)
        {
            // Check if is the ending delimeter, if it is reset all paremeters
            reset_barcode_params();
            vTaskDelete(NULL);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
