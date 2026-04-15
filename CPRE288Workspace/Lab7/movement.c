/*
 * movement.c
 *
 *  Created on: Feb 4, 2026
 *      Author: milajune
 */
#include "open_interface.h"
#include "lcd.h"
#include <string.h>

void bumpLeft(oi_t *sensor_data);
void bumpRight(oi_t *sensor_data);

double move_forward(oi_t *sensor_data, double distance_mm){
    double sum = 0;

    if (distance_mm > 0) {
        oi_setWheels (150,150);

        while (sum < distance_mm){
            oi_update(sensor_data);
            sum += (*sensor_data).distance;

            if ((*sensor_data).bumpLeft){
                sum += 15; //was -150, is +15
                bumpLeft(sensor_data);
                oi_setWheels(150, 150);
            } else if ((*sensor_data).bumpRight){
                sum += 15;
                bumpRight(sensor_data);
                oi_setWheels(150, 150);
            }

            //char string_sum[] = (char)sum;
            //lcd_printf("%lf", sum);
        }
    } else {
        oi_setWheels(-150, -150);

        while (sum > distance_mm) {
            oi_update(sensor_data);
            sum += (*sensor_data).distance;
        }
    }

    oi_setWheels(0,0);
    return sum;
}

double turn_right(oi_t *sensor_data, double degrees){

    double sum = 0.0395*degrees;
    oi_setWheels (-35,35);

    while (sum < degrees){
        oi_update(sensor_data);
        sum -= (*sensor_data).angle;
        //lcd_printf("%lf", sum);
    }

    oi_setWheels(0,0);

    return sum;
}

double turn_left(oi_t *sensor_data, double degrees){

    double sum = 0.0395*degrees;
    oi_setWheels (35,-35);

    while (sum < degrees){
        oi_update(sensor_data);
        sum+= (*sensor_data).angle;
        //lcd_printf("%lf", sum);
    }

    oi_setWheels(0,0);

    return sum;
}

void bumpLeft(oi_t *sensor_data) {

    move_forward(sensor_data, -150);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 250);
    turn_left(sensor_data, 90);
    move_forward(sensor_data, 150);
}

void bumpRight(oi_t *sensor_data) {

    move_forward(sensor_data, -150);
    turn_left(sensor_data, 90);
    move_forward(sensor_data, 250);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 150);
}




