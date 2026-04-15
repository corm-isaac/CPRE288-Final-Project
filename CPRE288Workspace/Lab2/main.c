/*
 * main.c
 *
 *  Created on: Feb 4, 2026
 *      Author: milajune
 */

  //cybot 16

#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"

int main (void) {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    timer_init();

    // move 1 meter forward
    //move_forward(sensor_data, 1000);

    /*
    move_forward(sensor_data, 500);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 500);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 500);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 500);
    */
    move_forward(sensor_data,2000);
    //double distance = move_forward(sensor_data, 2000);
    //lcd_printf("Distance: %f", distance);

    oi_free(sensor_data);
}





