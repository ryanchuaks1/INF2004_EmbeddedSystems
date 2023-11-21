#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"

enum cgiMessage
{
    CGI_MAIN_MENU,
    CGI_CALIB_WHEELS,
    CGI_START_CAR,
    CGI_NULL_MESSAGE
};

enum cgiMessageValue
{
    CGI_ZERO,
    CGI_TEN,
    CGI_TWENTY,
    CGI_THIRTY,
    CGI_FORTY,
    CGI_FIFTY,
    CGI_SIXTY,
    CGI_SEVENTY,
    CGI_EIGHTY,
    CGI_NINETY,
    CGI_HUNDRED,
    CGI_NULL_VALUE
};

enum cgiMessageDirection
{
    CGI_FORWARD,
    CGI_BACKWARD,
    CGI_LEFT,
    CGI_RIGHT,
    CGI_STOP,
    CGI_NULL_DIRECTION
};

extern enum cgiMessage message_cgi = CGI_NULL_MESSAGE;
extern enum cgiMessageValue message_cgi_value = CGI_NULL_VALUE;
extern enum cgiMessageDirection message_cgi_direction = CGI_NULL_DIRECTION;

// CGI handler which is run when a request for /led.cgi is detected
const char *cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Check if an request for LED has been made (/led.cgi?led=x)
    if (strcmp(pcParam[0], "led") == 0)
    {
        // Look at the argument to check if LED is to be turned on (x=1) or off (x=0)
        if (strcmp(pcValue[0], "0") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        else if (strcmp(pcValue[0], "1") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    }

    // Send the index page back to the user
    return "/index.shtml";
}

// CGI handler which is run when a request for /led.cgi is detected
const char *cgi_movement_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    if (strcmp(pcParam[0], "move") == 0)
    {
        if (strcmp(pcValue[0], "0") == 0)
            message_cgi = CGI_MAIN_MENU;
        else if (strcmp(pcValue[0], "1") == 0)
            message_cgi = CGI_CALIB_WHEELS;
        else if (strcmp(pcValue[0], "2") == 0)
            message_cgi = CGI_START_CAR;
    }

    // Send the index page back to the user
    return "/index.shtml";
}

const char *cgi_number_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    if (strcmp(pcParam[0], "num") == 0)
    {
        if (strcmp(pcValue[0], "0") == 0)
            message_cgi_value = CGI_ZERO;
        else if (strcmp(pcValue[0], "1") == 0)
            message_cgi_value = CGI_TEN;
        else if (strcmp(pcValue[0], "2") == 0)
            message_cgi_value = CGI_TWENTY;
        else if (strcmp(pcValue[0], "3") == 0)
            message_cgi_value = CGI_THIRTY;
        else if (strcmp(pcValue[0], "4") == 0)
            message_cgi_value = CGI_FORTY;
        else if (strcmp(pcValue[0], "5") == 0)
            message_cgi_value = CGI_FIFTY;
        else if (strcmp(pcValue[0], "6") == 0)
            message_cgi_value = CGI_SIXTY;
        else if (strcmp(pcValue[0], "7") == 0)
            message_cgi_value = CGI_SEVENTY;
        else if (strcmp(pcValue[0], "8") == 0)
            message_cgi_value = CGI_EIGHTY;
        else if (strcmp(pcValue[0], "9") == 0)
            message_cgi_value = CGI_NINETY;
        else if (strcmp(pcValue[0], "10") == 0)
            message_cgi_value = CGI_HUNDRED;
    }

    // Send the index page back to the user
    return "/index.shtml";
}

// tCGI Struct
// Fill this with all of the CGI requests and their respective handlers
static const tCGI cgi_handlers[] = {
    {
        "/led.cgi", cgi_led_handler // CGI request for "/led.cgi" with handler cgi_led_handler
    },
    {
        "/move.cgi", cgi_movement_handler // CGI request for "/movement" with handler cgi_movement_handler
    },
    {
        "/num.cgi", cgi_number_handler // CGI request for "/number" with handler cgi_number_handler
    }};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 3);
}