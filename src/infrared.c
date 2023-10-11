/*
 * Author: Rene Low
 * Description: TODO: Add description
 */

#include "../include/infrared.h"

void ir_sensor_init()
{
    // TODO: init sensor here
}

bool ir_sensor_read(enum Direction dir)
{
    switch (dir)
    {
    case LEFT:
        // TODO: read left IR
        return false;
        break;
    case RIGHT:
        // TODO: read right IR
        return false;
        break;
    case FRONT:
        // TODO: read front IR
        return false;
        break;
    default:
        return false;
    }
}
