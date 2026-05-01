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
#include "boundary.h"

int main (void) {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    timer_init();

    //MAIN BOUNDARY TEST
    while(1){
        int ret = checkBoundary(sensor_data);
        lcd_printf(ret);
    }

    oi_free(sensor_data);
}





