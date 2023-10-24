/*
 * Author: Ryan Chua
 * Description: TODO: Add description
 */

#include "../include/barcode.h"

void barcode_init()
{
    // Initialize the ADC
    adc_init();
    adc_gpio_init(BARCODE_SENSOR_PIN);
    adc_select_input(BARCODE_ADC_CHANNEL);
    isBlackBar = false;
    barType = THICK_WHITE_BAR;
}

void check_front_IR_intensity()
{
    uint16_t raw_value = adc_read();
    // printf("%d\n", raw_value);

    if (raw_value < THICK_WHITE && barType != THICK_WHITE_BAR)
    {
        resetChecker = 0;
        isBlackBar = false;
        barType = THICK_WHITE_BAR;
        barResults[resultCount] = THICK_WHITE_BAR;
        resultCount++;
        printf("Replaced with thick white\n");
    }
    else if (raw_value < THIN_WHITE && isBlackBar && barType != THICK_WHITE_BAR && barType != THIN_WHITE_BAR)
    {
        resetChecker = 0;
        isBlackBar = false;
        barType = THIN_WHITE_BAR;
        barResults[resultCount] = THIN_WHITE_BAR;
        resultCount++;
        printf("Thin white\n");
    }
    else if (raw_value > THICK_BLACK && barType != THICK_BLACK_BAR)
    {
        resetChecker = 0;
        isBlackBar = true;
        barType = THICK_BLACK_BAR;
        barResults[resultCount] = THICK_BLACK_BAR;
        resultCount++;
        printf("Reaplced with Thick black\n");
    }
    else if (raw_value > THIN_BLACK && !isBlackBar && barType != THICK_BLACK_BAR && barType != THIN_BLACK_BAR)
    {
        resetChecker = 0;
        isBlackBar = true;
        barType = THIN_BLACK_BAR;
        barResults[resultCount] = THIN_BLACK_BAR;
        resultCount++;
        printf("Thin black\n");
    }

    // Check if barcode is complete (for example, after 9 results)
    if (resultCount == 9)
    {
        printf("Barcode: ");
        for (int i = 0; i < resultCount; i++)
        {
            printf("%d", barResults[i]);
        }
        printf("\n");

        // Reset resultCount for the next barcode
        resultCount = 0;
    }

    resetChecker++;
    if (resetChecker > 200)
    {
        printf("Reset Occured\n");
        resultCount = 0;
        resetChecker = 0;
    }
}
